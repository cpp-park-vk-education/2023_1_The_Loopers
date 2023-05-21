#include "ievents_handler.h"

#include <ibase_service_chassis.h>

namespace inklink::base_service_chassis
{
class IBaseServiceChassis;
}

namespace inklink::service_message_broker
{
IEventsHandler::IEventsHandler(std::unique_ptr<IBaseServiceChassis>& chassis) : m_serviceChassis{chassis}
{
}

void IEventsHandler::RemoveSubscriber(int eventType, const Endpoint& endpoint)
{
    if (m_subscribers.contains(eventType)) [[likely]]
    {
        std::erase(m_subscribers[eventType], endpoint);
    }
}

void IEventsHandler::AddSubscriber(int eventType, const Endpoint& endpoint)
{
    m_subscribers[eventType].push_back(endpoint);
}

void IEventsHandler::SendEvent(int, const std::string&)
{
    // TODO
}

} // namespace inklink::service_message_broker
