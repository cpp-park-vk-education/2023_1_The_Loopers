#pragma once

#include "InternalSessionsManager.h"

#include <memory>
#include <string>

namespace inklink::external_service_chassis
{
class IUserExitHandler
{
public:
    virtual ~IExternalServiceChassis() = default;

    virtual void SetSessionManager(std::shared_ptr<IInternalSessionsManager>) = 0;
    virtual void DoOnUserExit(const std::string&) = 0;

protected:
    std::shared_ptr<IInternalSessionsManager> m_sessionManger;
};
} // namespace inklink::external_service_chassis
