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
    std::chrono::time_point<std::chrono::system_clock> time;
    int posStart;
    int posEnd;
    IData* data;
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
