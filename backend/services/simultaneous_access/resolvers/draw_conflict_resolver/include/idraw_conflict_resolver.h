#pragma once

#include "inklink_global.h"

#include <chrono>
#include <map>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

namespace inklink::serializer
{
class IData;
}

namespace inklink::service_simultaneous_access
{
struct DrawAction
{
    using IData = serializer::IData;

    ResolverActionType type;
    std::string figureId;
    Endpoint endpoint;
    std::chrono::time_point<std::chrono::system_clock> time;
    IData* data;

    [[nodiscard]] constexpr bool operator==(const DrawAction& other) const noexcept
    {
        return std::tie(type, figureId, endpoint, time, data) ==
               std::tie(other.type, other.figureId, other.endpoint, other.time, other.data);
    }
    [[nodiscard]] constexpr bool operator!=(const DrawAction& other) const noexcept
    {
        return !(*this == other);
    }
};

class IDrawConflictResolver
{
    using time_point = std::chrono::time_point<std::chrono::system_clock>;

public:
    virtual ~IDrawConflictResolver() = default;

    // vector by value because a lot of implementation will change it => no additional copies from const&
    virtual void Resolve(std::vector<DrawAction>&) = 0;
    [[nodiscard]] std::vector<DrawAction> GetHistory() const
    {
        return m_history;
    }

protected:
    std::vector<DrawAction> m_history;
};
} // namespace inklink::service_simultaneous_access
