#pragma once

#include "ibase_service_chassis.h"
#include "iuser_exit_handler.h"

#include <memory>
#include <string>

namespace inklink::external_service_chassis
{
class IUserExitHandler;

struct IExternalServiceChassis
{
    using IBaseServiceChassis = base_service_chassis::IBaseServiceChassis;

public:
    virtual ~IExternalServiceChassis() = default;

    std::unique_ptr<IBaseServiceChassis> baseServiceChassis;
    std::unique_ptr<IUserExitHandler> userExitHandler;
};
} // namespace inklink::external_service_chassis
