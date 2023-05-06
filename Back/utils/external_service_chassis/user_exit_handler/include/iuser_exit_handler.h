#ifndef _IUSEREXITHANDLER_H_
#define _IUSEREXITHANDLER_H_

#include <memory>
#include <string>

#include "InternalSessionsManager.h"

class IUserExitHandler {
  public:
    virtual ~IExternalServiceChassis() = default;

    virtual void SetSessionManager(std::shared_ptr<IInternalSessionsManager>) {
    }
    virtual void DoOnUserExit(const std::string&) {
    }

  protected:
    std::shared_ptr<IInternalSessionsManager> m_sessionManger;
};

#endif  // _IUSEREXITHANDLER_H_
