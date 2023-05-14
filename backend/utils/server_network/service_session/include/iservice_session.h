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
    explicit IServiceSession(std::shared_ptr<IInternalSessionsManager> manager,
                             std::shared_ptr<IAuthorizer> auth) noexcept
            : m_manager{std::move(manager)}, m_authorizer{std::move(auth)}
    {
    }
    virtual ~IServiceSession() = default;

    virtual Endpoint GetClientEndpoint() = 0;

    virtual void RunAsync() = 0;
    virtual void Send(const std::string&) = 0;

protected:
    std::shared_ptr<IInternalSessionsManager> m_manager;
    std::shared_ptr<IAuthorizer> m_authorizer;
};
} // namespace inklink::server_network
