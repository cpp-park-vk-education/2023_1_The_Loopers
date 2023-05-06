#ifndef _SIMULTANEOUSACCESSSERVICE_H_
#define _SIMULTANEOUSACCESSSERVICE_H_

#include <memory>
#include <string>

#include "idb_adapter.h"
#include "iexternal_service_chassis.h"
#include "iresolvers_factory.h"

class SimultaneousAccessService {
  public:
    virtual void SetResolversFactory(IResolversFactory*) {
    }
    virtual void SetServiceChassis(IExternalServiceChassis*) {
    }
    virtual void SetDbAdapter(IDBAdapter*) {
    }

    virtual void run(unsigned short port) {
    }

  private:
    virtual void DoOnConnect() {
    }
    virtual void DoOnRead(const string&, IServiceSession*) {
    }
    virtual void DoOnWrite() {
    }
};

#endif  // _SIMULTANEOUSACCESSSERVICE_H_
