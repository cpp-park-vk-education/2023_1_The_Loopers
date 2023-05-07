#ifndef _IDRAWCONFLICTRESOLVER_H_
#define _IDRAWCONFLICTRESOLVER_H_

#include <chrono>
#include <memory>
#include <string>
#include <vector>

#include "global.h"

class IData;

struct DrawAction {
    constexpr bool operator==(const DrawAction&) const noexcept {
        return true;
    }
    ResolverActionType type;
    std::chrono::time_point<std::chrono::system_clock> time;
    IData* data;
};

class IDrawConflictResolver {
  public:
    virtual ~IDrawConflictResolver() = default;

    virtual std::vector<DrawAction> resolve(std::vector<DrawAction>) {
        return {};
    }
};

#endif  // _IDRAWCONFLICTRESOLVER_H_
