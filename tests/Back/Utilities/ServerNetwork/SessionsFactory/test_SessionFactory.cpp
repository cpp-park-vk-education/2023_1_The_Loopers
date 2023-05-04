#ifndef _ISESSIONSFACTORY_H_
#define _ISESSIONSFACTORY_H_

#include <memory>
#include <string>

#include "IServiceSession.h"

class IInternalSessionsManager;
class INetworkStreamAdapter;

class ISessionsFactory {
  public:
    virtual ~ISessionsFactory() = default;

    virtual void SetManager(InternalSessionsManager*) {
    }
    virtual void SetAuthorizer(Authorizer*) {
    }
    virtual void SetNetworkAdapter(NetworkStreamAdapter*) {
    }

    virtual IServiceSession* GetSession(asio::ip::tcp::socket&&) {
    }
};

#endif  // _ISERVICESESSION_H_
