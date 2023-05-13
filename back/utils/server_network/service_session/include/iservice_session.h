#ifndef _ISERVICESESSION_H_
#define _ISERVICESESSION_H_

#include <memory>
#include <string>

#include "global.h"

namespace inklink_authorizer
{
class IAuthorizer;
}
namespace inklink_service_chassiss
{
class InternalSessionsManager;
}
// class INetworkStreamAdapter;

namespace inklink_service_session
{
class IServiceSession
{
public:
    using InternalSessionsManager = inklink_service_chassiss::InternalSessionsManager;
    using IAuthorizer = inklink_authorizer::IAuthorizer;

    virtual ~IServiceSession() = default;

    virtual void SetManager(std::shared_ptr<InternalSessionsManager>);
    virtual void SetAuthorizer(std::shared_ptr<IAuthorizer>);
    // virtual void SetNetworkAdapter(std::shared_ptr<INetworkStreamAdapter>);
    virtual Endpoint GetClientEndpoint();

    virtual void run_async()
    {
    }
    virtual std::string sync_read()
    {
    }
    virtual void send(const std::string&)
    {
    }

protected:
    Endpoint m_endpoint;

    std::shared_ptr<IAuthorizer> m_authorizer;
    std::shared_ptr<InternalSessionsManager> m_manager;
    // std::shared_ptr<INetworkStreamAdapter> m_adapter;
};
}  // namespace inklink_service_session

#endif  // _ISERVICESESSION_H_
