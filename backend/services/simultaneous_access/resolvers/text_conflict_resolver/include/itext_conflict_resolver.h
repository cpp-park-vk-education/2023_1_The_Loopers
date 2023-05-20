#pragma once

#include "inklink_global.h"

#include <chrono>
#include <memory>
#include <string>
#include <vector>

namespace inklink::serializer
{
class IData;
}

namespace inklink::service_simultaneous_access
{
struct TextAction
{
    using IData = serializer::IData;

    ResolverActionType type;
    std::string figureId;
    Endpoint endpoint;
    std::chrono::time_point<std::chrono::system_clock> time;
    int posStart;
    int posEnd;
    IData* data;

    [[nodiscard]] constexpr bool operator==(const TextAction& other) const noexcept
    {
        return std::tie(type, figureId, endpoint, time, posStart, posEnd, data) ==
               std::tie(other.type, other.figureId, other.endpoint, other.time, other.posStart, other.posEnd,
                        other.data);
    }
    [[nodiscard]] constexpr bool operator!=(const TextAction& other) const noexcept
    {
        return !(*this == other);
    }
};

class ITextConflictResolver
{
public:
    virtual ~ITextConflictResolver() = default;

    virtual void Resolve(std::vector<TextAction>&) const = 0;
};
} // namespace inklink::service_simultaneous_access
