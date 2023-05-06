#ifndef _ILISTENER_H_
#define _ILISTENER_H_

#include <memory>
#include <string>

#include "isessions_factory.h"

class IListener {
  public:
    virtual ~IListener() = default;

    virtual void run() {
    }
    virtual void async_run() {
    }
    virtual void SetSessionFactory(ISessionFactory*) {
    }
};

#endif  // _ILISTENER_H_
