#pragma once

#include "inklink_global.h"

#include <ibase_service_chassis.h>
#include <isessions_factory.h>

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

#include <functional>
#include <memory>
#include <string>

namespace inklink::chassis_configurator
{
using namespace base_service_chassis;
class BaseChassisWebsocketConfigurator
{
public:
    using io_context = boost::asio::io_context;
    using error_code = boost::system::error_code;

    using BeastWebsocketListener = server_network::BeastWebsocketListener;
    using ISessionsFactory = server_network::ISessionsFactory;
    using IServiceSession = server_network::IServiceSession;

    using NotifiedFunctor = std::function<void(int /*event type*/, const std::string&, const Endpoint& /*from*/)>;
    using ReadFunctor = std::function<void(const std::string&)>;

public:
    ~BaseChassisWebsocketConfigurator() = delete;

    // clang-format off
    [[nodiscard]] static std::unique_ptr<IBaseServiceChassis>
    CreateAndInitializeFullChassis(
        const std::string& loggerName, 
        const std::string& logFilePath, 
        // TODO (a.novak) add in factory separate method SetSessionsManager (to be able to create factory without 
        // manager and pass it do not pass manager here)
        io_context& ioContext, 
        std::unique_ptr<ISessionsFactory> factory, // for listener
        std::shared_ptr<InternalSessionsManager> manager, // specified in factory: needed in constructor
        ServiceType typeSelf, 
        const Endpoint& endpointSelf, // to register in registry
        NotifiedFunctor notifiedCallback, // event from message broker
        ReadFunctor readCallback // signal from message broker
    );

    [[nodiscard]] static std::unique_ptr<IBaseServiceChassis>
    CreateAndInitializeChassisWithoutMsgBroker(
        const std::string& loggerName, 
        const std::string& logFilePath, 
        // TODO (a.novak) same as line 40
        io_context& ioContext, 
        std::unique_ptr<ISessionsFactory> factory, // for listener
        std::shared_ptr<InternalSessionsManager> manager, // specified in factory: needed in constructor
        ServiceType typeSelf, 
        const Endpoint& endpointSelf // to register in registry
    );

    [[nodiscard]] static std::unique_ptr<IBaseServiceChassis>
    CreateAndInitializeChassisWithoutRegistratorAndMsgBroker(
        const std::string& loggerName, 
        const std::string& logFilePath, 
        // TODO (a.novak) same as line 40 (add in factory separate method SetSessionsManager)
        io_context& ioContext, 
        std::unique_ptr<ISessionsFactory> factory, // for listener
        std::shared_ptr<InternalSessionsManager> manager, // specified in factory: needed in constructor
        const Endpoint& endpointSelf
    );
    // clang-format on

private:
    [[nodiscard]] static std::shared_ptr<ILogger> CreateLogger(const std::string& loggerName,
                                                               const std::string& logFilePath);

    static void AddListener(const std::unique_ptr<IBaseServiceChassis>& chassis, io_context& ioContext,
                            std::unique_ptr<ISessionsFactory> factory, const Endpoint& endpointSelf);

    static void AddServiceRegistrator(std::unique_ptr<IBaseServiceChassis>& chassis, ServiceType typeSelf,
                                      const Endpoint& endpointSelf);

    static void AddMsgBrokerConnection(std::unique_ptr<IBaseServiceChassis>& chassis, ServiceType typeSelf,
                                       const Endpoint& endpointSelf,
                                       NotifiedFunctor notifiedCallback, // event from message broker
                                       ReadFunctor readCallback          // signal from message broker
    );
};
} // namespace inklink::chassis_configurator
