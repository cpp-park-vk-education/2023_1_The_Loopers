#pragma once

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

namespace inklink::chassis_configurator
{
using namespace base_service_chassis;
class BaseChassisWebsocketConfigurator
{
    using io_context = boost::asio::io_context;
    using error_code = boost::system::error_code;

    using BeastWebsocketListener = server_network::BeastWebsocketListener;
    using ISessionsFactory = server_network::ISessionsFactory;
    using IServiceSession = server_network::IServiceSession;

public:
    ~BaseChassisWebsocketConfigurator() = delete;

    // clang-format off
    [[nodiscard]] static std::unique_ptr<IBaseServiceChassis>
    CreateAndInitializeFullChassis(
        const std::string& loggerName, const std::string& logFilePath, 
        // TODO (a.novak) add in factory separate method SetSessionsManager
        io_context& ioContext, std::unique_ptr<ISessionsFactory> factory, // for listener
        std::shared_ptr<InternalSessionsManager> manager, // specified in factory: needed in constructor
        ServiceType typeSelf, const Endpoint& endpointSelf, // to register in registry
        std::function<void(int /*event type*/, const std::string&, Endpoint /*from*/)> 
                      notifiedCallback, // event from message broker
        std::function<void(const std::string&)> readCallback // signal from message broker
    )
    // clang-format on
    {
        auto chassis = CreateAndInitializeChassisWithoutMsgBroker(loggerName, logFilePath, ioContext,
                                                                  std::move(factory), manager, typeSelf, endpointSelf);

        // add connection to msg broker
        AddMsgBrokerConnection(chassis, typeSelf, endpointSelf, notifiedCallback, readCallback);

        return chassis;
    }

    // clang-format off
    [[nodiscard]] static std::unique_ptr<IBaseServiceChassis>
    CreateAndInitializeChassisWithoutMsgBroker(
        const std::string& loggerName, const std::string& logFilePath, 
        // TODO (a.novak) same as line 40
        io_context& ioContext, std::unique_ptr<ISessionsFactory> factory, // for listener
        std::shared_ptr<InternalSessionsManager> manager, // specified in factory: needed in constructor
        ServiceType typeSelf, const Endpoint& endpointSelf // to register in registry
    )
    // clang-format on
    {
        auto chassis = CreateAndInitializeChassisWithoutRegistratorAndMsgBroker(
                loggerName, logFilePath, ioContext, std::move(factory), manager, endpointSelf);

        // register at service registry
        AddServiceRegistrator(chassis, typeSelf, endpointSelf);

        return chassis;
    }

    // clang-format off
    [[nodiscard]] static std::unique_ptr<IBaseServiceChassis>
    CreateAndInitializeChassisWithoutRegistratorAndMsgBroker(
        const std::string& loggerName, const std::string& logFilePath, 
        // TODO (a.novak) same as line 40
        io_context& ioContext, std::unique_ptr<ISessionsFactory> factory, // for listener
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

private:
    [[nodiscard]] static std::shared_ptr<ILogger> CreateLogger(const std::string& loggerName,
                                                               const std::string& logFilePath)
    {
        // TODO (a.novak) logger: to config
        auto logger = std::make_shared<SpdlogAdapter>();
        logger->SetFile(loggerName, logFilePath);
        logger->SetLevel(LogLevel::kDebug);
        return logger;
    }

    static void AddListener(std::unique_ptr<IBaseServiceChassis>& chassis, io_context& ioContext,
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

    static void AddServiceRegistrator(std::unique_ptr<IBaseServiceChassis>& chassis, ServiceType typeSelf,
                                      const Endpoint& endpointSelf)
    {
        chassis->registrator = std::make_unique<WebsocketServiceRegistrator>(chassis->logger);
        if (!chassis->registrator->Register(typeSelf, endpointSelf))
        {
            chassis->logger->LogCritical("Unable to register in registry on startup! Stopping.");
            throw std::runtime_error("Unable to register in registry on startup! Stopping.");
        }
    }

    static void AddMsgBrokerConnection(
            std::unique_ptr<IBaseServiceChassis>& chassis, ServiceType typeSelf, const Endpoint& endpointSelf,
            std::function<void(int, const std::string&, Endpoint)> notifiedCallback, // event from message broker
            std::function<void(const std::string&)> readCallback                     // signal from message broker
    )
    {
        auto msgBrokerServices = chassis->registrator->GetEndpoints(ServiceType::kMessageBroker);

        const int kMaxAttempts{10};
        int attempt{0};
        while (msgBrokerServices.empty() && attempt <= kMaxAttempts)
        {
            std::this_thread::sleep_for(1s);
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
};
} // namespace inklink::chassis_configurator
