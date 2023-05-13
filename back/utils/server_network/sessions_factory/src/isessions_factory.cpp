#include "isessions_factory.h"

namespace inklink_sessions_factory
{
void ISessionsFactory::SetManager(std::shared_ptr<InternalSessionsManager> manager)
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
}  // namespace inklink_sessions_factory
