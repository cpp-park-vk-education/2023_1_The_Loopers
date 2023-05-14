#pragma once

#include "iservice_session.h"

#include <memory>
#include <string>

namespace boost::asio::ip::tcp
{
class socket;
}

class IAuthorizer;
class IInternalSessionsManager;
class INetworkStreamAdapter;

namespace inklink::server_network
{
class ISessionsFactory
{
public:
    virtual ~ISessionsFactory() = default;

    virtual void SetManager(IInternalSessionsManager*);
    virtual void SetAuthorizer(std::shared_ptr<IAuthorizer>);
    virtual void SetNetworkAdapter(std::shared_ptr<INetworkStreamAdapter>);

    virtual IServiceSession* GetSession(boost::asio::ip::tcp::socket&&) = 0;

protected:
    std::shared_ptr<IAuthorizer> m_authorizer;
    IInternalSessionsManager* m_manager;
    std::shared_ptr<INetworkStreamAdapter> m_adapter;
};
} // namespace inklink::server_network
