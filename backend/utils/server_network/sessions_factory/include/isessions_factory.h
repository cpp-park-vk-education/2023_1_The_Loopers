#pragma once

#include "iservice_session.h"

#include <memory>
#include <string>

namespace boost::asio::ip::tcp
{
class socket;
}

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

public:
    virtual ~ISessionsFactory() = default;

    virtual void SetManager(IInternalSessionsManager*);
    virtual void SetAuthorizer(std::shared_ptr<IAuthorizer>);

    virtual IServiceSession* GetSession(boost::asio::ip::tcp::socket&&) = 0;

protected:
    std::shared_ptr<IAuthorizer> m_authorizer;
    IInternalSessionsManager* m_manager;
};
} // namespace inklink::server_network
