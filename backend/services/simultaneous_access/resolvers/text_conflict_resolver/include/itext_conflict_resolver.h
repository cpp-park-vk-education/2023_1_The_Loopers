#pragma once

#include "global.h"

#include <chrono>
#include <memory>
#include <string>
#include <vector>

class IData;

struct TextAction
{
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

    virtual std::vector<TextAction> resolve(std::vector<TextAction>)
    {
    }
};
