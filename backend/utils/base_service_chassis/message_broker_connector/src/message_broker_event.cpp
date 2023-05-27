#include "message_broker_event.h"

#include "ilogger.h"

#include <data_container.h>
#include <iclient_session.h>

#include <json_serializer.h>

#include <boost/system/error_code.hpp>

#include <algorithm>
#include <sstream>

namespace
{
using IClientSession = inklink::client_connector::IClientSession;
using error_code = boost::system::error_code;
using Endpoint = inklink::Endpoint;

using NotifiedFunctor = std::function<void(int /*event type*/, const std::string&, const Endpoint& /*from*/)>;

using DataContainer = inklink::serializer::DataContainer;
using JsonSerializer = inklink::serializer::JsonSerializer;
} // namespace

namespace inklink::base_service_chassis
{
MessageBrokerEvent::MessageBrokerEvent(std::shared_ptr<ICommonConnection> cc, NotifiedFunctor notifiedCallback,
                                       std::shared_ptr<ILogger> logger)
        : m_connectionToMsgBroker{cc}, m_notifiedCallback{notifiedCallback}, m_logger{logger}
{
    m_connectionToMsgBroker->AddReadCallback(
            [this](const std::string& msgBrokerEvent, error_code ec, IClientSession* session)
            { DoOnNotified(msgBrokerEvent, ec, session); });
}

void MessageBrokerEvent::Publish(int event, const std::string& msgBody, ServiceType subscribersType)
{
    auto session = m_connectionToMsgBroker->GetSession();

    if (!session) [[unlikely]]
    {
        std::stringstream ss{};
        auto msgTruncated = msgBody.substr(0, 50);
        std::replace(msgTruncated.begin(), msgTruncated.end(), '\n', ' ');
        ss << "No connection to MsgBroker. Tried publishing event of type '" << event << "', sending to '"
           << static_cast<int>(subscribersType) << "' with msg '" << msgTruncated << "' ";
        m_logger->LogDebug(ss.str());
        return;
    }

    DataContainer eventMsg{};
    eventMsg["event"] = event;
    eventMsg["message_body"] = msgBody;
    eventMsg["time"] = std::string("now");
    const std::string serializedMsg = JsonSerializer::SerializeAsString(eventMsg);
    session->Send(serializedMsg);
}

void MessageBrokerEvent::Subscribe(int event)
{
    auto session = m_connectionToMsgBroker->GetSession();

    if (!session) [[unlikely]]
    {
        std::stringstream ss{};
        ss << "No connection to MsgBroker. Tried subscribing to events of type '" << event << "'.";
        m_logger->LogDebug(ss.str());
        return;
    }

    DataContainer eventMsg{};
    eventMsg["event"] = event;
    eventMsg["sender"]["address"] = session->GetSelfEndpoint().address;
    eventMsg["sender"]["port"] = static_cast<int>(session->GetSelfEndpoint().port);
    eventMsg["time"] = std::string("now");
    const std::string serializedMsg = JsonSerializer::SerializeAsString(eventMsg);
    session->Send(serializedMsg);
}

void MessageBrokerEvent::DoOnNotified(const std::string& msgBody, error_code ec, IClientSession*) const
{
    if (ec) [[unlikely]]
    {
        std::stringstream ss{};
        // TODO (a.novak) <<session.GetEndpoint() when will add overload
        ss << "Error occurred while reading from msgBroker." << ec.message();

        m_logger->LogDebug(ss.str());
        return;
    }

    DataContainer eventMsg{JsonSerializer::ParseFromString(msgBody)};
    if (!eventMsg.Has("message_body") || !eventMsg.Has("event") || !eventMsg.Has("sender") ||
        !eventMsg["sender"].Has("address") || !eventMsg["sender"].Has("port"))
    {
        m_logger->LogWarning(std::string("Got msg with invalid format.") + msgBody);
        return;
    }
    const std::string newMsgBody = eventMsg.AsString("message_body");
    const int eventType = eventMsg.AsInt("event");
    const Endpoint sender{.address = eventMsg["sender"].AsString("address"),
                          .port = static_cast<std::uint16_t>(eventMsg["sender"].AsInt("port"))};
    m_notifiedCallback(eventType, newMsgBody, sender);
}

} // namespace inklink::base_service_chassis
