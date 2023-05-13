#include "websocket_service_session.h"

namespace inklink_service_session
{
void IServiceSession::SetManager(IInternalSessionsManager* manager)
{
    m_manager = manager;
}

void IServiceSession::SetAuthorizer(std::shared_ptr<IAuthorizer> authorizer)
{
    m_authorizer = authorizer;
}

void IServiceSession::SetNetworkAdapter(std::shared_ptr<INetworkStreamAdapter> adapter)
{
    m_adapter = adapter;
}
}  // namespace inklink_service_session
