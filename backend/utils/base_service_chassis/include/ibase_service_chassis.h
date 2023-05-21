#pragma once

#include "ilocal_service_registry.h"
#include "ilogger.h"
#include "imessage_broker_event.h"
#include "imessage_broker_signal.h"
#include "inklink_global.h"
#include "internal_sessions_manager.h"
#include "iservice_registrator.h"

// #include "iserializer.h"

#include <ilistener.h>

#include <memory>
#include <string>
#include <vector>

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
public:
    std::shared_ptr<ILogger> logger;
    std::unique_ptr<server_network::IListener> listener;
    std::unique_ptr<IServiceRegistrator> registrator;
    std::unique_ptr<IMessageBrokerSignal> signalBroker;
    std::unique_ptr<IMessageBrokerEvent> eventBroker;
    std::unique_ptr<ILocalServiceRegistry> localServiceRegistry;
    std::unique_ptr<InternalSessionsManager> internalSessionsManager;
    // std::shared_ptr<ISerializer> m_serializer{std::make_shared<ISerializer>()};

    // virtual ~IBaseServiceChassis() = default;
};
} // namespace inklink::base_service_chassis
