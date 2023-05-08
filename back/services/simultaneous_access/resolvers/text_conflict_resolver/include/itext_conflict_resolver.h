#ifndef _ITEXTCONFLICTRESOLVER_H_
#define _ITEXTCONFLICTRESOLVER_H_

#include <chrono>
#include <memory>
#include <string>
#include <vector>

#include "global.h"

class IData;

struct TextAction
{
    ResolverActionType type;
    std::string figureId;
    Endpoint endpoint;
    std::chrono::time_point<std::chrono::system_clock> time;
    int posStart;
    int posEnd;
    IData *data;

    bool operator==(const TextAction &other) const
    {
        return type == other.type && figureId == other.figureId && userLogin == other.userLogin &&
               time == other.time && posStart == other.posStart && posEnd == other.posEnd &&
               data == other.data;
    }
    bool operator!=(const TextAction &other) const
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

#endif  // _ITEXTCONFLICTRESOLVER_H_
