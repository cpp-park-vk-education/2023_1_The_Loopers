#include "websocket_common_connection.h"

#include "icommon_connection.h"

namespace
{
using IClientSession = inklink::client_connector::IClientSession;
}

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
