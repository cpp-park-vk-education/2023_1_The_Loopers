#ifndef _ISERVICESESSION_H_
#define _ISERVICESESSION_H_

#include <memory>
#include <string>

#include "global.h"

class IAuthorizer;
class IInternalSessionsManager;
class INetworkStreamAdapter;

class IServiceSession
{
public:
    virtual ~IServiceSession() = default;

    virtual void SetManager(IInternalSessionsManager*);
    virtual void SetAuthorizer(std::shared_ptr<IAuthorizer>);
    virtual void SetNetworkAdapter(std::shared_ptr<INetworkStreamAdapter>);
    virtual Endpoint GetClientEndpoint()
    {
    }

    virtual void run()
    {
    }
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
    std::shared_ptr<IAuthorizer> m_authorizer;
    IInternalSessionsManager* m_manager;
    std::shared_ptr<INetworkStreamAdapter> m_adapter;
};

#endif  // _ISERVICESESSION_H_
