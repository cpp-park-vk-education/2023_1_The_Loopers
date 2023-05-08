#include "isessions_factory.h"

void ISessionsFactory::SetManager(IInternalSessionsManager* manager)
{
    m_manager = manager;
}

void ISessionsFactory::SetAuthorizer(std::shared_ptr<IAuthorizer> authorizer)
{
    m_authorizer = authorizer;
}

void ISessionsFactory::SetNetworkAdapter(std::shared_ptr<INetworkStreamAdapter> adapter)
{
    m_adapter = adapter;
}
