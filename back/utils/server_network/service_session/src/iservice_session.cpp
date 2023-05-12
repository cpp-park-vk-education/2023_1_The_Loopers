#include "iservice_session.h"

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
