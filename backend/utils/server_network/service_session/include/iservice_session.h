#pragma once

#include "inklink_global.h"

#include <memory>
#include <string>

namespace inklink::authorizer
{
class IAuthorizer;
}

namespace inklink::base_service_chassis
{
class IInternalSessionsManager;
}

namespace inklink::server_network
{
class IServiceSession
{
    using IAuthorizer = authorizer::IAuthorizer;
    using IInternalSessionsManager = base_service_chassis::IInternalSessionsManager;

public:
    virtual ~IServiceSession() = default;

    virtual void SetManager(IInternalSessionsManager*);
    virtual void SetAuthorizer(std::shared_ptr<IAuthorizer>);
    virtual Endpoint GetClientEndpoint() = 0;

    virtual void RunAsync() = 0;
    virtual void Send(const std::string&) = 0;

protected:
    std::shared_ptr<IAuthorizer> m_authorizer;
    IInternalSessionsManager* m_manager;
};
} // namespace inklink::server_network
