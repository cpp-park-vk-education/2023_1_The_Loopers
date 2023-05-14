#pragma once

#include <memory>
#include <string>

namespace inklink::base_service_chassis
{
class IInternalSessionsManager;
}

namespace inklink::external_service_chassis
{
class IUserExitHandler
{
    using IInternalSessionsManager = base_service_chassis::IInternalSessionsManager;

public:
    explicit IUserExitHandler(std::shared_ptr<IInternalSessionsManager> manager) noexcept
            : m_sessionManger{std::move(manager)}
    {
    }
    virtual ~IUserExitHandler() = default;

    virtual void DoOnUserExit(const std::string&) = 0;

protected:
    std::shared_ptr<IInternalSessionsManager> m_sessionManger;
};
} // namespace inklink::external_service_chassis
