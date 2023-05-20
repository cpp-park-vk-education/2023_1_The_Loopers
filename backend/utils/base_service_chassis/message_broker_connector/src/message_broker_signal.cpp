#include "message_broker_signal.h"

#include <algorithm>
#include <sstream>

namespace
{
using IClientSession = inklink::client_connector::IClientSession;
using error_code = boost::system::error_code;
} // namespace

namespace inklink::base_service_chassis
{

MessageBrokerSignal::MessageBrokerSignal(
        std::shared_ptr<ICommonConnection> cc,
        std::function<void(const std::string&, error_code, IClientSession*)> readCallback,
        std::shared_ptr<ILogger> logger)
        : m_connectionToMsgBroker{cc}, m_doOnRead{readCallback}, m_logger{logger}
{
    m_connectionToMsgBroker->AddReadCallback(m_doOnRead);
}

// virtual void SetDoOnRead(std::function<void(const std::string&, error_code, IClientSession*)>);
void MessageBrokerSignal::Request(const std::string& msgBody, const Endpoint& requestFrom)
{
    auto session = m_connectionToMsgBroker->GetSession();

    if (!session) [[unlikely]]
    {
        std::stringstream ss{};
        auto msgTruncated = msgBody.substr(0, 50);
        std::replace(msgTruncated.begin(), msgTruncated.end(), '\n', ' ');
        // TODO (a.novak) <<endpoint when will add overload
        ss << "No connection to MsgBroker. Tried requesting from '"
           << "' with msg " << msgTruncated;
        m_logger->LogDebug(ss.str());

        return;
    }

    // TODO (a.novak) wrap str and endpoint into message broker protocol
    std::string serializedMsg = msgBody + requestFrom.address;
    session->Send(serializedMsg);
}

void MessageBrokerSignal::Send(const std::string& msgBody, const Endpoint& sendTo)
{
    auto session = m_connectionToMsgBroker->GetSession();

    if (!session) [[unlikely]]
    {
        std::stringstream ss{};
        auto msgTruncated = msgBody.substr(0, 50);
        std::replace(msgTruncated.begin(), msgTruncated.end(), '\n', ' ');
        // TODO (a.novak) <<endpoint when will add overload
        ss << "No connection to MsgBroker. Tried sending to '"
           << "' with msg " << msgTruncated;

        m_logger->LogDebug(ss.str());
        return;
    }

    // TODO (a.novak) wrap str and endpoint into message broker protocol
    std::string serializedMsg = msgBody + sendTo.address;
    session->Send(serializedMsg);
}

} // namespace inklink::base_service_chassis
