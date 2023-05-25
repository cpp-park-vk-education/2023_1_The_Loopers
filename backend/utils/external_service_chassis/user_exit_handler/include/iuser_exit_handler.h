#pragma once

#include <memory>
#include <string>

namespace inklink::base_service_chassis
{
class InternalSessionsManager;
}

namespace inklink::external_service_chassis
{
class IUserExitHandler
{
    using InternalSessionsManager = base_service_chassis::InternalSessionsManager;

public:
    explicit IUserExitHandler(std::shared_ptr<InternalSessionsManager> manager) noexcept : m_sessionManger{manager}
    {
    }
    virtual ~IUserExitHandler() = default;

    // TODO stub for now
    virtual void DoOnUserExit(const std::string&)
    {
    }

protected:
    std::shared_ptr<InternalSessionsManager> m_sessionManger;
};
} // namespace inklink::external_service_chassis
