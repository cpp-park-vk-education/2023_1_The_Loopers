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
    m_serviceType = type;
    m_endpointSelf = self;
    m_endpointOther = other;

    auto session = std::make_shared<
            WebsocketClientSession<decltype(m_acceptCallback), decltype(m_readCallback), decltype(m_writeCallback)>>(
            m_ioContext, self.address, self.port, m_acceptCallback, m_readCallback, m_writeCallback);
    session->RunAsync(other.address, other.port);
    m_session = session;

    m_ioContextExecutor =
            boost::asio::require(m_ioContext.get_executor(), boost::asio::execution::outstanding_work.tracked);

    m_threadIoContext = std::thread([this]() { this->m_ioContext.run(); });
}

void WebsocketCommonConnection::ChangeConnection(ServiceType, const Endpoint& self, const Endpoint& other)
{
    m_serviceType = type;
    m_endpointSelf = self;
    m_endpointOther = other;
}

std::shared_ptr<IClientSession> WebsocketCommonConnection::GetSession() const noexcept
{
    return m_session.lock();
}

} // namespace inklink::base_service_chassis
