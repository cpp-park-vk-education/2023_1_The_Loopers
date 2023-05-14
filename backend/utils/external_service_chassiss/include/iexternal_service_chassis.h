#pragma once

#include "BaseServicechassiss.h"
#include "UserExitHandler.h"

#include <memory>
#include <string>

namespace inklink::external_service_chassis
{
class IExternalServiceChassis
{
public:
    virtual ~IExternalServiceChassis() = default;

    std::shared_ptr<IBaseServiceChassis> m_baseServiceChassis;
    std::shared_ptr<IUserExitHandler> m_userExitHandler;
};
} // namespace inklink::external_service_chassis
