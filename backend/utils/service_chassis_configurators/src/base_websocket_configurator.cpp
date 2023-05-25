#include "inklink/chassis_configurators/base_websocket_configurator.h"

#include "inklink_global.h"

#include <ibase_service_chassis.h>
#include <isessions_factory.h>
#include <spdlog_adapter.h>
#include <websocket_service_registrator.h>

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

#include <chrono>
#include <exception>
#include <functional>
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
using ServiceType = inklink::ServiceType;
using Endpoint = inklink::Endpoint;

using BeastWebsocketListener = inklink::server_network::BeastWebsocketListener;
using ISessionsFactory = inklink::server_network::ISessionsFactory;
using IServiceSession = inklink::server_network::IServiceSession;

using NotifiedFunctor = std::function<void(int /*event type*/, const std::string&, const inklink::Endpoint& /*from*/)>;
using ReadFunctor = std::function<void(const std::string&)>;

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
    return logger;
}

void AddListener(const std::unique_ptr<IBaseServiceChassis>& chassis, io_context& ioContext,
                 std::unique_ptr<ISessionsFactory> factory, const Endpoint& endpointSelf)
{
    auto listener = std::make_shared<BeastWebsocketListener>(
            ioContext, {endpointSelf.address, endpointSelf.port}, std::move(factory),
            [&chassis](error_code ec, IServiceSession*)
            {
                if (!ec) [[likely]]
                {
                    chassis->logger->LogInfo("Accepted new connection to ''");
                }
            });
    listener->AsyncRun();
    chassis->listener = listener;
}

void AddServiceRegistrator(std::unique_ptr<IBaseServiceChassis>& chassis, ServiceType typeSelf,
                           const Endpoint& endpointSelf)
{
    chassis->registrator = std::make_unique<WebsocketServiceRegistrator>(chassis->logger);
    if (!chassis->registrator->Register(typeSelf, endpointSelf))
    {
        chassis->logger->LogCritical("Unable to register in registry on startup! Stopping.");
        throw std::runtime_error("Unable to register in registry on startup! Stopping.");
    }
}

void AddMsgBrokerConnection(std::unique_ptr<IBaseServiceChassis>& chassis, ServiceType typeSelf,
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

    chassis->signalBroker = std::make_unique<MessageBrokerSignal>(commonConnection, readCallback);
    chassis->eventBroker = std::make_unique<MessageBrokerEvent>(commonConnection, notifiedCallback);
}

} // namespace

namespace inklink::chassis_configurator
{
// clang-format off
std::unique_ptr<base_service_chassis::IBaseServiceChassis>
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

    return chassis;
}

// clang-format off
std::unique_ptr<IBaseServiceChassis>
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

    return chassis;
}

// clang-format off
std::unique_ptr<IBaseServiceChassis>
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
    auto chassis = std::make_unique<IBaseServiceChassis>();

    // first configure logger
    chassis->logger = CreateLogger(loggerName, logFilePath);

    chassis->internalSessionsManager = manager;

    // start listening network
    AddListener(chassis, ioContext, std::move(factory), endpointSelf);

    // no service registrator

    // no connection to message broker

    // add local service registry
    chassis->localServiceRegistry = std::make_unique<LocalServiceRegistry>();

    return chassis;
}
} // namespace inklink::chassis_configurator