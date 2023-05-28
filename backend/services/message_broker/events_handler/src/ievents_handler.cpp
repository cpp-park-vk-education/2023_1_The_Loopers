#include "ievents_handler.h"

#include <data_container.h>
#include <ibase_service_chassis.h>

#include <json_serializer.h>

namespace
{
using DataContainer = inklink::serializer::DataContainer;
using JsonSerializer = inklink::serializer::JsonSerializer;
} // namespace

namespace inklink::service_message_broker
{
IEventsHandler::IEventsHandler(IBaseServiceChassis& chassis) : m_serviceChassis{chassis}
{
}

bool IEventsHandler::Handle(const serializer::DataContainer& msgData, const Endpoint& sender)
{
    if (msgData.Has("event") && msgData.Has("message_body"))
    {
        DataContainer newMsg{};
        newMsg["sender"]["address"] = sender.address;
        newMsg["sender"]["port"] = static_cast<int>(sender.port);
        newMsg["event"] = msgData.AsInt("event");
        newMsg["message_body"] = msgData["message_body"];
        newMsg["time"] = std::string("now");
        SendEvent(msgData.AsInt("event"), JsonSerializer::SerializeAsString(newMsg));
        return true;
    }
    if (msgData.Has("event") && msgData.Has("sender"))
    {
        if (!msgData["sender"].Has("address") || !msgData["sender"].Has("port"))
        {
            return false;
        }
        const Endpoint newSubscriber{.address = msgData["sender"].AsString("address"),
                                     .port = static_cast<std::uint16_t>(msgData["sender"].AsInt("port"))};
        AddSubscriber(msgData.AsInt("event"), newSubscriber);
        return true;
    }
    return false;
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

void IEventsHandler::SendEvent(int eventType, const std::string& msg)
{
    for (const auto& subscriber : m_subscribers[eventType])
    {
        auto session = m_serviceChassis.internalSessionsManager->GetSession(subscriber).lock();
        if (session)
        {
            session->Send(msg);
        }
    }
}

} // namespace inklink::service_message_broker
