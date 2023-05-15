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

    bool operator==(const TextAction& other) const
    {
        return std::tie(type, figureId, endpoint, time, posStart, posEnd, data) ==
               std::tie(other.type, other.figureId, other.endpoint, other.time, other.posStart, other.posEnd,
                        other.data);
    }
    bool operator!=(const TextAction& other) const
    {
        return !(*this == other);
    }
};

class ITextConflictResolver
{
public:
    virtual ~ITextConflictResolver() = default;

    virtual std::vector<TextAction> Resolve(std::vector<TextAction>) = 0;
};
} // namespace inklink::service_simultaneous_access
