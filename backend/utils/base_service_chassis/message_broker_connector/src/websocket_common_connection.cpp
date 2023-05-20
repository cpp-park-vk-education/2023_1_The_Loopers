#include "websocket_common_connection.h"

#include "icommon_connection.h"

#include <manual_websocket_client_session.h>

#include <thread>

namespace
{
using IClientSession = inklink::client_connector::IClientSession;
using ConnectType = inklink::client_connector::ConnectType;
using error_code = boost::system::error_code;
} // namespace
namespace inklink::base_service_chassis
{

WebsocketCommonConnection::WebsocketCommonConnection(std::shared_ptr<ILogger> logger) : ICommonConnection{logger}
{
}

void WebsocketCommonConnection::Init(ServiceType type, const Endpoint& self, const Endpoint& other)
{
    ChangeConnection(type, self, other);

    m_ioContextExecutor =
            boost::asio::require(m_ioContext.get_executor(), boost::asio::execution::outstanding_work.tracked);

    m_threadIoContext = std::thread([this]() { this->m_ioContext.run(); });
}

void WebsocketCommonConnection::ChangeConnection(ServiceType type, const Endpoint& self, const Endpoint& other)
{
    m_serviceType = type;
    m_endpointSelf = self;
    m_endpointOther = other;

    auto sessionOld = m_session.lock();
    if (sessionOld) [[likely]]
    {
        sessionOld->Close();
    }

    auto onAccept = [this](ConnectType type, error_code ec, IClientSession* session)
    {
        for (auto callback : m_acceptCallbacks)
        {
            callback(type, ec, session);
        }
    };
    auto onRead = [this](const std::string& str, error_code ec, IClientSession* session)
    {
        for (auto callback : m_readCallbacks)
        {
            callback(str, ec, session);
        }
    };
    auto onWrite = [this](error_code ec)
    {
        for (auto callback : m_writeCallbacks)
        {
            callback(ec);
        }
    };

    auto session = std::make_shared<WebsocketClientSession<decltype(onAccept), decltype(onRead), decltype(onWrite)>>(
            m_ioContext, self.address, self.port, onAccept, onRead, onWrite);
    session->RunAsync(other.address, other.port);
    m_session = session;

    // TODO (a.novak) probably send smth about that we are connected
}

std::shared_ptr<IClientSession> WebsocketCommonConnection::GetSession() const noexcept
{
    return m_session.lock();
}

} // namespace inklink::base_service_chassis
