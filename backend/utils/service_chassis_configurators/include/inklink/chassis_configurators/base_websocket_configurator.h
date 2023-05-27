#pragma once

#include "inklink_global.h"

#include <ibase_service_chassis.h>
#include <isessions_factory.h>

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

#include <filesystem>
#include <functional>
#include <memory>
#include <string>

namespace inklink::chassis_configurator
{
class BaseChassisWebsocketConfigurator
{
public:
    using io_context = boost::asio::io_context;
    using error_code = boost::system::error_code;

    using InternalSessionsManager = base_service_chassis::InternalSessionsManager;
    using IBaseServiceChassis = base_service_chassis::IBaseServiceChassis;
    using ILogger = base_service_chassis::ILogger;

    using ISessionsFactory = server_network::ISessionsFactory;
    using IServiceSession = server_network::IServiceSession;

    using NotifiedFunctor = std::function<void(int /*event type*/, const std::string&, const Endpoint& /*from*/)>;
    using ReadFunctor = std::function<void(const std::string&)>;

public:
    ~BaseChassisWebsocketConfigurator() = delete;

    // clang-format off
    [[nodiscard]] static std::shared_ptr<base_service_chassis::IBaseServiceChassis>
    CreateAndInitializeFullChassis(
        const std::string& loggerName, 
        const std::filesystem::path& logFilePath, 
        // TODO (a.novak) add in factory separate method SetSessionsManager (to be able to create factory without 
        // manager and pass it do not pass manager here)
        io_context& ioContext, 
        std::unique_ptr<ISessionsFactory> factory, // for listener
        std::shared_ptr<InternalSessionsManager> manager, // specified in factory: needed in constructor
        ServiceType typeSelf, 
        const Endpoint& endpointSelfAsServer,
        const Endpoint& endpointSelfAsClient, // to register in registry
        NotifiedFunctor notifiedCallback, // event from message broker
        ReadFunctor readCallback // signal from message broker
    );

    [[nodiscard]] static std::shared_ptr<IBaseServiceChassis>
    CreateAndInitializeChassisWithoutMsgBroker(
        const std::string& loggerName, 
        const std::filesystem::path& logFilePath, 
        // TODO (a.novak) same as line 40
        io_context& ioContext, 
        std::unique_ptr<ISessionsFactory> factory, // for listener
        std::shared_ptr<InternalSessionsManager> manager, // specified in factory: needed in constructor
        ServiceType typeSelf, 
        const Endpoint& endpointSelfAsServer,
        const Endpoint& endpointSelfAsClient // to register in registry
    );

    [[nodiscard]] static std::shared_ptr<IBaseServiceChassis>
    CreateAndInitializeChassisWithoutRegistratorAndMsgBroker(
        const std::string& loggerName, 
        const std::filesystem::path& logFilePath, 
        // TODO (a.novak) same as line 40 (add in factory separate method SetSessionsManager)
        io_context& ioContext, 
        std::unique_ptr<ISessionsFactory> factory, // for listener
        std::shared_ptr<InternalSessionsManager> manager, // specified in factory: needed in constructor
        const Endpoint& endpointSelfAsServer
    );
    // clang-format on
};
} // namespace inklink::chassis_configurator
