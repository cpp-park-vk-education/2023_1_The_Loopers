#pragma once

#include "ibase_service_chassiss.h"
#include "iuser_exit_handler.h"

#include <memory>
#include <string>

namespace inklink::external_service_chassis
{
class IUserExitHandler;

class IExternalServiceChassis
{
    using IBaseServiceChassis = base_service_chassis::IBaseServiceChassiss;
public:
    virtual ~IExternalServiceChassis() = default;

    std::shared_ptr<IBaseServiceChassis> m_baseServiceChassis;
    std::shared_ptr<IUserExitHandler> m_userExitHandler;
};
} // namespace inklink::external_service_chassis
