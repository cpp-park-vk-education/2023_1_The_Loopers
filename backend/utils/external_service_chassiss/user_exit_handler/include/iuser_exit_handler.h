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

    virtual void SetSessionManager(std::shared_ptr<IInternalSessionsManager>)
    {
    }
    virtual void DoOnUserExit(const std::string&)
    {
    }

protected:
    std::shared_ptr<IInternalSessionsManager> m_sessionManger;
};
} // namespace inklink::external_service_chassis
