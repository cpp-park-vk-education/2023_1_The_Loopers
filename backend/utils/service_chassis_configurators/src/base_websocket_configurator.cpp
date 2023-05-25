#include "inklink/chassis_configurators/base_websocket_configurator.h"

#include "inklink_global.h"

#include <beast_websocket_listener.h>
#include <ibase_service_chassis.h>
#include <isessions_factory.h>
#include <spdlog_adapter.h>
#include <websocket_common_connection.h>
#include <websocket_service_registrator.h>

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

#include <chrono>
#include <exception>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>

namespace
{
using io_context = boost::asio::io_context;
using error_code = boost::system::error_code;

using IBaseServiceChassis = inklink::base_service_chassis::IBaseServiceChassis;
using ILogger = inklink::base_service_chassis::ILogger;
using LocalServiceRegistry = inklink::base_service_chassis::LocalServiceRegistry;
using WebsocketServiceRegistrator = inklink::base_service_chassis::WebsocketServiceRegistrator;
using WebsocketCommonConnection = inklink::base_service_chassis::WebsocketCommonConnection;
using MessageBrokerSignal = inklink::base_service_chassis::MessageBrokerSignal;
using MessageBrokerEvent = inklink::base_service_chassis::MessageBrokerEvent;
namespace server_network = inklink::server_network;

using ServiceType = inklink::ServiceType;
using Endpoint = inklink::Endpoint;

using ISessionsFactory = inklink::server_network::ISessionsFactory;
using IServiceSession = inklink::server_network::IServiceSession;

using NotifiedFunctor = std::function<void(int /*event type*/, const std::string&, const inklink::Endpoint& /*from*/)>;
using ReadFunctor = std::function<void(const std::string&)>;

namespace ip = boost::asio::ip;

using namespace std::chrono_literals;

constexpr int kMaxAttempts{10};

constexpr std::chrono::seconds kDelay{1s};

[[nodiscard]] std::shared_ptr<ILogger> CreateLogger(const std::string& loggerName,
                                                    const std::filesystem::path& logFilePath)
{
    // TODO (a.novak) logger: to config
    auto logger = std::make_shared<inklink::base_service_chassis::SpdlogAdapter>();
    logger->SetFile(loggerName, logFilePath.string());
    logger->SetLevel(inklink::base_service_chassis::LogLevel::kDebug);
    logger->LogDebug("Logger has been set");
    return logger;
}

void AddListener(const std::shared_ptr<IBaseServiceChassis> chassis, io_context& ioContext,
                 std::unique_ptr<ISessionsFactory> factory, const Endpoint& endpointSelf)
{
    auto lamOnAccept = [chassis](error_code ec, IServiceSession*)
    {
        if (!ec) [[likely]]
        {
            chassis->logger->LogInfo("Accepted new connection to ''");
        }
    };

    auto localAddress = ip::address::from_string(endpointSelf.address);
    auto localEndpoint = ip::tcp::endpoint{localAddress, static_cast<unsigned short>(endpointSelf.port)};

    auto listener = std::make_shared<server_network::BeastWebsocketListener<decltype(lamOnAccept)>>(
            ioContext, localEndpoint, std::move(factory), lamOnAccept);
    listener->AsyncRun();
    chassis->listener = listener;
}

void AddServiceRegistrator(std::shared_ptr<IBaseServiceChassis> chassis, ServiceType typeSelf,
                           const Endpoint& endpointSelf)
{
    chassis->registrator = std::make_unique<WebsocketServiceRegistrator>(chassis->logger);
    if (!chassis->registrator->Register(typeSelf, endpointSelf))
    {
        std::cout << "Unable to register in registry on startup!" << __LINE__ << std::endl;
        chassis->logger->LogCritical("Unable to register in registry on startup! Stopping.");
        throw std::runtime_error("Unable to register in registry on startup! Stopping.");
    }
}

void AddMsgBrokerConnection(std::shared_ptr<IBaseServiceChassis> chassis, ServiceType typeSelf,
                            const Endpoint& endpointSelf,
                            NotifiedFunctor notifiedCallback, // event from message broker
                            ReadFunctor readCallback          // signal from message broker
)
{
    auto msgBrokerServices = chassis->registrator->GetEndpoints(ServiceType::kMessageBroker);

    int attempt{0};
    while (msgBrokerServices.empty() && attempt <= kMaxAttempts)
    {
        std::this_thread::sleep_for(kDelay);
        msgBrokerServices = chassis->registrator->GetEndpoints(ServiceType::kMessageBroker);
        ++attempt;
    }
    if (msgBrokerServices.empty()) [[unlikely]]
    {
        // TODO (a.novak) add tryConnect to msgBroker to chassis
        // it is ok not to have connection to message broker. In this case in future work service should try to
        // connect to message broker "explicitly", when it's needed
        chassis->logger->LogWarning("Unable to establish connection to message broker on startup!");
        // return;
        throw std::runtime_error("No connection to message broker. Stopping");
    }

    auto commonConnection = std::make_shared<WebsocketCommonConnection>(chassis->logger);
    // in future may be multiple services
    commonConnection->Init(typeSelf, endpointSelf, msgBrokerServices[0]);

    chassis->signalBroker = std::make_unique<MessageBrokerSignal>(commonConnection, readCallback, chassis->logger);
    chassis->eventBroker = std::make_unique<MessageBrokerEvent>(commonConnection, notifiedCallback, chassis->logger);
}

} // namespace

namespace inklink::chassis_configurator
{
// clang-format off
std::shared_ptr<base_service_chassis::IBaseServiceChassis>
BaseChassisWebsocketConfigurator::CreateAndInitializeFullChassis(
    const std::string& loggerName, 
    const std::filesystem::path& logFilePath, 
    // TODO (a.novak) add in factory separate method SetSessionsManager (to be able to create factory without 
    // manager and pass it do not pass manager here)
    io_context& ioContext, 
    std::unique_ptr<ISessionsFactory> factory, // for listener
    std::shared_ptr<InternalSessionsManager> manager, // specified in factory: needed in constructor
    ServiceType typeSelf, 
    const Endpoint& endpointSelf, // to register in registry
    NotifiedFunctor notifiedCallback, // event from message broker
    ReadFunctor readCallback // signal from message broker
)
// clang-format on
{
    auto chassis = CreateAndInitializeChassisWithoutMsgBroker(loggerName, logFilePath, ioContext, std::move(factory),
                                                              manager, typeSelf, endpointSelf);

    // add connection to msg broker
    AddMsgBrokerConnection(chassis, typeSelf, endpointSelf, notifiedCallback, readCallback);

    std::cout << "AddMsgBrokerConnection" << __LINE__ << std::endl;

    return chassis;
}

// clang-format off
std::shared_ptr<IBaseServiceChassis>
BaseChassisWebsocketConfigurator::CreateAndInitializeChassisWithoutMsgBroker(
    const std::string& loggerName, 
    const std::filesystem::path& logFilePath, 
    // TODO (a.novak) same as line 40
    io_context& ioContext, 
    std::unique_ptr<ISessionsFactory> factory, // for listener
    std::shared_ptr<InternalSessionsManager> manager, // specified in factory: needed in constructor
    ServiceType typeSelf, 
    const Endpoint& endpointSelf // to register in registry
)
// clang-format on
{
    auto chassis = CreateAndInitializeChassisWithoutRegistratorAndMsgBroker(loggerName, logFilePath, ioContext,
                                                                            std::move(factory), manager, endpointSelf);

    // register at service registry
    AddServiceRegistrator(chassis, typeSelf, endpointSelf);

    std::cout << "AddServiceRegistrator" << __LINE__ << std::endl;

    return chassis;
}

// clang-format off
std::shared_ptr<IBaseServiceChassis>
BaseChassisWebsocketConfigurator::CreateAndInitializeChassisWithoutRegistratorAndMsgBroker(
    const std::string& loggerName, 
    const std::filesystem::path& logFilePath, 
    // TODO (a.novak) same as line 40 (add in factory separate method SetSessionsManager)
    io_context& ioContext, 
    std::unique_ptr<ISessionsFactory> factory, // for listener
    std::shared_ptr<InternalSessionsManager> manager, // specified in factory: needed in constructor
    const Endpoint& endpointSelf
)
// clang-format on
{
    auto chassis = std::make_shared<IBaseServiceChassis>();

    // first configure logger
    chassis->logger = CreateLogger(loggerName, logFilePath);

    std::cout << "Logger created" << __LINE__ << std::endl;

    chassis->internalSessionsManager = manager;

    std::cout << "Manager set" << __LINE__ << std::endl;

    // start listening network
    AddListener(chassis, ioContext, std::move(factory), endpointSelf);

    std::cout << "Listener added" << __LINE__ << std::endl;

    // no service registrator

    // no connection to message broker

    // add local service registry
    chassis->localServiceRegistry = std::make_unique<LocalServiceRegistry>();

    std::cout << "Add service registry" << __LINE__ << std::endl;

    return chassis;
}
} // namespace inklink::chassis_configurator
