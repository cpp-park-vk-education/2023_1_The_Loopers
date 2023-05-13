#include "iservice_session.h"

namespace inklink_service_session
{
void IServiceSession::SetManager(std::shared_ptr<InternalSessionsManager> manager)
{
    m_manager = manager;
}

void IServiceSession::SetAuthorizer(std::shared_ptr<IAuthorizer> authorizer)
{
    m_authorizer = authorizer;
}

// void IServiceSession::SetNetworkAdapter(std::shared_ptr<INetworkStreamAdapter> adapter)
// {
//     m_adapter = adapter;
// }

Endpoint IServiceSession::GetClientEndpoint()
{
    return m_endpoint;
}
}  // namespace inklink_service_session
