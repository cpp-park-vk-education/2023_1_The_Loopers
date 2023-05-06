#ifndef _ISERVICESESSION_H_
#define _ISERVICESESSION_H_

#include <memory>
#include <string>

#include "iauthorizer.h"

class IInternalSessionsManager;
class INetworkStreamAdapter;

class IServiceSession {
  public:
    virtual ~IServiceSession() = default;

    virtual void SetManager(IInternalSessionsManager*) {
    }
    virtual void SetAuthorizer(IAuthorizer*) {
    }
    virtual void SetNetworkAdapter(INetworkStreamAdapter*) {
    }

    virtual void run() {
    }
    virtual void run_async() {
    }
    virtual std::string sync_read() {
    }
    virtual void send(const std::string&) {
    }
};

#endif  // _ISERVICESESSION_H_
