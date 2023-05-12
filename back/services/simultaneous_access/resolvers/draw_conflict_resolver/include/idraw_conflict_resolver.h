#ifndef _IDRAWCONFLICTRESOLVER_H_
#define _IDRAWCONFLICTRESOLVER_H_

#include <chrono>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "global.h"

class IData;

struct DrawAction
{
    ResolverActionType type;
    std::string figureId;
    Endpoint endpoint;
    std::chrono::time_point<std::chrono::system_clock> time;
    IData *data;

    constexpr bool operator==(const DrawAction &other) const noexcept
    {
        return type == other.type && figureId == other.figureId && endpoint == other.endpoint &&
               time == other.time && data == other.data;
    }
    bool operator!=(const DrawAction &other) const
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

#endif  // _IDRAWCONFLICTRESOLVER_H_
