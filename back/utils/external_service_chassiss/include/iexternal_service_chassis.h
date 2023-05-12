#ifndef _IEXTERNALSERVICECHASSIS_H_
#define _IEXTERNALSERVICECHASSIS_H_

#include <memory>
#include <string>

#include "BaseServicechassiss.h"
#include "UserExitHandler.h"

class IExternalServiceChassis {
  public:
    virtual ~IExternalServiceChassis() = default;

    std::shared_ptr<IBaseServiceChassis> m_baseServiceChassis;
    std::shared_ptr<IUserExitHandler> m_userExitHandler;
};

#endif  // _IEXTERNALSERVICECHASSIS_H_
