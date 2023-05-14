#pragma once

#include "global.h"

#include <chrono>
#include <map>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

class IData;

namespace inklink::service_simultaneous_access
{
struct DrawAction
{
    ResolverActionType type;
    std::string figureId;
    Endpoint endpoint;
    std::chrono::time_point<std::chrono::system_clock> time;
    IData* data;

    constexpr bool operator==(const DrawAction& other) const noexcept
    {
        return std::tie(type, figureId, endpoint, time, data) ==
               std::tie(other.type, other.figureId, other.endpoint, other.time, other.data);
    }
    bool operator!=(const DrawAction& other) const
    {
        return !(*this == other);
    }
};

class IDrawConflictResolver
{
public:
    virtual ~IDrawConflictResolver() = default;

    virtual std::vector<DrawAction> resolve(std::vector<DrawAction>)
    {
        return {};
    }
};
} // namespace inklink::service_simultaneous_access
