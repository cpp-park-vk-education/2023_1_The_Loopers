#pragma once

#include "global.h"

#include <memory>
#include <string>

class IAuthorizer;
class IInternalSessionsManager;
class INetworkStreamAdapter;

namespace inklink::server_network
{
class IServiceSession
{
public:
    virtual ~IServiceSession() = default;

    virtual void SetManager(IInternalSessionsManager*);
    virtual void SetAuthorizer(std::shared_ptr<IAuthorizer>);
    virtual void SetNetworkAdapter(std::shared_ptr<INetworkStreamAdapter>);
    virtual Endpoint GetClientEndpoint() = 0;

    virtual void run() = 0;
    virtual void run_async() = 0;
    virtual std::string sync_read() = 0;
    virtual void send(const std::string&) = 0;

protected:
    std::shared_ptr<IAuthorizer> m_authorizer;
    IInternalSessionsManager* m_manager;
    std::shared_ptr<INetworkStreamAdapter> m_adapter;
};
} // namespace inklink::server_network
