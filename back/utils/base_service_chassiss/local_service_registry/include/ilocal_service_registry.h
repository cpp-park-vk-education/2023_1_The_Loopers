#ifndef _ILOCALSERVICEREGISTRY_H_
#define _ILOCALSERVICEREGISTRY_H_

#include <string>
#include <vector>

#include "global.h"

class ILocalServiceRegistry {
  public:
    virtual ~ILocalServiceRegistry() = default;

    virtual void AddService(ServiceType,const Endpoint&) {
    }

    virtual std::vector<Endpoint> GetServices(ServiceType) {
    }
};

#endif  // _ILOCALSERVICEREGISTRY_H_
