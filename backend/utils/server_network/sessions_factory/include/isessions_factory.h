#ifndef _ISESSIONSFACTORY_H_
#define _ISESSIONSFACTORY_H_

#include <memory>
#include <string>

#include "iservice_session.h"

namespace boost::asio::ip::tcp
{
class socket;
}

class IAuthorizer;
class IInternalSessionsManager;
class INetworkStreamAdapter;

class ISessionsFactory
{
public:
    virtual ~ISessionsFactory() = default;

    virtual void SetManager(IInternalSessionsManager*);
    virtual void SetAuthorizer(std::shared_ptr<IAuthorizer>);
    virtual void SetNetworkAdapter(std::shared_ptr<INetworkStreamAdapter>);

    virtual IServiceSession* GetSession(boost::asio::ip::tcp::socket&&)
    {
    }

protected:
    std::shared_ptr<IAuthorizer> m_authorizer;
    IInternalSessionsManager* m_manager;
    std::shared_ptr<INetworkStreamAdapter> m_adapter;
};

#endif  // _ISERVICESESSION_H_
