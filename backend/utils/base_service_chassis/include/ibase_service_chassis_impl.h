#pragma once

#include <memory>

namespace inklink::server_network
{
class IListener;
} // namespace inklink::server_network

namespace inklink::base_service_chassis
{
class ILogger;
class IServiceRegistrator;
class IMessageBrokerSignal;
class IMessageBrokerEvent;
class ILocalServiceRegistry;
class InternalSessionsManager;
} // namespace inklink::base_service_chassis

namespace inklink::base_service_chassis
{
/**
 * @class IBaseServiceChassis
 *
 * @brief BaseServiceChassis stores tools in on place
 *
 * @note May be in future it'll provide convience methods to log standard messages (now logging is done by tools itself,
 * which is not perfect because it can be duplicated in each of inherited classes) and to perform standard sequence of
 * operations (at the moment initialization done by configurator (it'll stay as is), but there may be some additional
 * sequences: for example add to localRegistry endpoints when got from global service registry)
 */
struct IBaseServiceChassis
{
    std::shared_ptr<ILogger> logger;
    std::weak_ptr<server_network::IListener> listener;
    std::unique_ptr<IServiceRegistrator> registrator;
    std::unique_ptr<IMessageBrokerSignal> signalBroker;
    std::unique_ptr<IMessageBrokerEvent> eventBroker;
    std::unique_ptr<ILocalServiceRegistry> localServiceRegistry;
    std::shared_ptr<InternalSessionsManager> internalSessionsManager;
};
} // namespace inklink::base_service_chassis
