#ifndef _ISERVICEREGISTRATOR_H_
#define _ISERVICEREGISTRATOR_H_

#include <memory>
#include <string>
#include <vector>

#include "global.h"

class IClientSession;

class IServiceRegistrator {
  public:
    virtual ~IServiceRegistrator() = default;

    virtual void SetConnection(IClientSession*) {
    }

    virtual bool Register(ServiceType,const Endpoint&) {
    }
    virtual void Deregister(ServiceType,const Endpoint&) {
    }
    virtual std::vector<Endpoint> GetEndpoints(ServiceType) {
    }

  protected:
    std::shared_ptr<IClientSession> m_cnnectionToRegistry;
};

#endif  // _ISERVICEREGISTRATOR_H_
