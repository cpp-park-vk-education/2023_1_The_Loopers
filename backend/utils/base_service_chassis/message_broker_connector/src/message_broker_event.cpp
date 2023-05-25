#include "message_broker_event.h"

#include "ilogger.h"

#include <iclient_session.h>

#include <boost/system/error_code.hpp>

#include <algorithm>
#include <sstream>

namespace
{
using IClientSession = inklink::client_connector::IClientSession;
using error_code = boost::system::error_code;
using Endpoint = inklink::Endpoint;

using NotifiedFunctor = std::function<void(int /*event type*/, const std::string&, const Endpoint& /*from*/)>;
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

    // TODO (a.novak) wrap str and endpoint into message broker protocol
    std::string serializedMsg = std::to_string(event) + msgBody + std::to_string(static_cast<int>(subscribersType));
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

    // TODO (a.novak) wrap str and endpoint into message broker protocol
    std::string serializedMsg = std::to_string(event);
    session->Send(serializedMsg);
}

void MessageBrokerEvent::DoOnNotified(const std::string& msgBody, error_code ec, IClientSession*) const
{
    if (ec) [[unlikely]]
    {
        std::stringstream ss{};
        // TODO (a.novak) <<session.GetEndpoint() when will add overload
        ss << "Error occurred while reading from msgBroker." << ec.what();
        m_logger->LogDebug(ss.str());
        return;
    }
    // TODO (a.novak) parse msgBrokerSignal
    std::string newMsgBody{msgBody};
    int eventType{};
    m_notifiedCallback(eventType, newMsgBody, m_connectionToMsgBroker->GetEndpointSelf());
}

} // namespace inklink::base_service_chassis
