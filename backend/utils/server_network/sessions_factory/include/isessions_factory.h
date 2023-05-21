#pragma once

#include "iservice_session.h"

#include <boost/asio/ip/tcp.hpp> // class tcp { class socket; } can't be forward declared

#include <memory>
#include <string>

namespace inklink::authorizer
{
class IAuthorizer;
}

namespace inklink::base_service_chassis
{
class InternalSessionsManager;
}

namespace inklink::server_network
{
class ISessionsFactory
{
    using IAuthorizer = authorizer::IAuthorizer;
    using InternalSessionsManager = base_service_chassis::InternalSessionsManager;
    using tcp = boost::asio::ip::tcp;

public:
    explicit ISessionsFactory(std::shared_ptr<InternalSessionsManager> manager,
                              std::shared_ptr<IAuthorizer> auth) noexcept
            : m_manager{std::move(manager)}, m_authorizer{std::move(auth)}
    {
    }
    virtual ~ISessionsFactory() = default;

    virtual std::shared_ptr<IServiceSession> GetSession(tcp::socket&&) = 0;

protected:
    std::shared_ptr<InternalSessionsManager> m_manager;
    std::shared_ptr<IAuthorizer> m_authorizer;
};
} // namespace inklink::server_network
