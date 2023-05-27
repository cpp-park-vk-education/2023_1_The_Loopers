#include "message_broker_signal.h"

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

using ReadFunctor = std::function<void(const std::string&)>;

using DataContainer = inklink::serializer::DataContainer;
using JsonSerializer = inklink::serializer::JsonSerializer;

std::string CreateLogMsg(std::string&& prefix, const std::string& msgBody, const inklink::Endpoint& requestFrom)
{
    std::stringstream ss{};
    auto msgTruncated = msgBody.substr(0, 50);
    std::replace(msgTruncated.begin(), msgTruncated.end(), '\n', ' ');
    // TODO (a.novak) <<endpoint when will add overload
    ss << prefix << " Endpoint: '" << requestFrom.port << "' with msg " << msgTruncated;
    return ss.str();
}
} // namespace

namespace inklink::base_service_chassis
{

MessageBrokerSignal::MessageBrokerSignal(std::shared_ptr<ICommonConnection> cc, ReadFunctor readCallback,
                                         std::shared_ptr<ILogger> logger)
        : m_connectionToMsgBroker{cc}, m_readCallback{readCallback}, m_logger{logger}
{
    m_connectionToMsgBroker->AddReadCallback(
            [this](const std::string& msgBrokerSignal, error_code ec, IClientSession* session)
            { DoOnRead(msgBrokerSignal, ec, session); });
}

// virtual void SetDoOnRead(std::function<void(const std::string&, error_code, IClientSession*)>);
void MessageBrokerSignal::Request(const std::string& msgBody, const Endpoint& requestFrom)
{
    auto session = m_connectionToMsgBroker->GetSession();

    if (!session) [[unlikely]]
    {
        m_logger->LogDebug(CreateLogMsg("No connection to MsgBroker.", msgBody, requestFrom));
        return;
    }

    DataContainer eventMsg{};
    eventMsg["receiver"]["address"] = requestFrom.address;
    eventMsg["receiver"]["port"] = static_cast<int>(requestFrom.port);
    eventMsg["message_body"] = msgBody;
    eventMsg["time"] = std::string("now");
    const std::string serializedMsg = JsonSerializer::SerializeAsString(eventMsg);
    session->Send(serializedMsg);
}

void MessageBrokerSignal::Send(const std::string& msgBody, const Endpoint& sendTo)
{
    auto session = m_connectionToMsgBroker->GetSession();

    if (!session) [[unlikely]]
    {
        m_logger->LogDebug(CreateLogMsg("No connection to MsgBroker.", msgBody, sendTo));
        return;
    }

    DataContainer eventMsg{};
    eventMsg["receiver"]["address"] = sendTo.address;
    eventMsg["receiver"]["port"] = static_cast<int>(sendTo.port);
    eventMsg["message_body"] = msgBody;
    eventMsg["time"] = std::string("now");
    const std::string serializedMsg = JsonSerializer::SerializeAsString(eventMsg);
    session->Send(serializedMsg);
}

void MessageBrokerSignal::DoOnRead(const std::string& msgBrokerSignal, error_code ec, IClientSession*) const
{
    if (ec) [[unlikely]]
    {
        std::stringstream ss{};
        // TODO (a.novak) <<session.GetEndpoint() when will add overload
        ss << "Error occurred while reading from msgBroker." << ec.message();
        m_logger->LogDebug(ss.str());
        return;
    }

    DataContainer eventMsg{JsonSerializer::ParseFromString(msgBrokerSignal)};
    if (!eventMsg.Has("message_body") || !eventMsg.Has("sender"))
    {
        m_logger->LogWarning(std::string("Got msg with invalid format.") + msgBrokerSignal);
        return;
    }
    m_readCallback(eventMsg.AsString("message_body"));
}

} // namespace inklink::base_service_chassis
