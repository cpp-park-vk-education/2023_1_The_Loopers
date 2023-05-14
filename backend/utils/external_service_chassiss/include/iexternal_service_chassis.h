#pragma once

#include "BaseServicechassiss.h"
#include "UserExitHandler.h"

#include <memory>
#include <string>


class IExternalServiceChassis
{
public:
    virtual ~IExternalServiceChassis() = default;

    std::shared_ptr<IBaseServiceChassis> m_baseServiceChassis;
    std::shared_ptr<IUserExitHandler> m_userExitHandler;
};
