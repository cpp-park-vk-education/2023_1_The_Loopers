#ifndef _ISESSIONSFACTORY_H_
#define _ISESSIONSFACTORY_H_

#include <boost/asio/ip/tcp.hpp>
#include <memory>
#include <string>

#include "iservice_session.h"

class IAuthorizer;
class InternalSessionsManager;
class INetworkStreamAdapter;

class ISessionsFactory
{
public:
    virtual ~ISessionsFactory() = default;

    virtual void SetManager(std::shared_ptr<InternalSessionsManager>);
    virtual void SetAuthorizer(std::shared_ptr<IAuthorizer>);
    virtual void SetNetworkAdapter(std::shared_ptr<INetworkStreamAdapter>);

    virtual std::shared_ptr<IServiceSession> GetSession(boost::asio::ip::tcp::socket&&)
    {
    }

protected:
    std::shared_ptr<IAuthorizer> m_authorizer;
    std::shared_ptr<InternalSessionsManager> m_manager;
    std::shared_ptr<INetworkStreamAdapter> m_adapter;
};

#endif  // _ISERVICESESSION_H_
