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
class IInternalSessionsManager;
}

namespace inklink::server_network
{
class ISessionsFactory
{
    using IAuthorizer = authorizer::IAuthorizer;
    using IInternalSessionsManager = base_service_chassis::IInternalSessionsManager;
    using tcp = boost::asio::ip::tcp;

public:
    virtual ~ISessionsFactory() = default;

    virtual void SetManager(IInternalSessionsManager*);
    virtual void SetAuthorizer(std::shared_ptr<IAuthorizer>);

    virtual IServiceSession* GetSession(tcp::socket&&) = 0;

protected:
    std::shared_ptr<IAuthorizer> m_authorizer;
    IInternalSessionsManager* m_manager;
};
} // namespace inklink::server_network
