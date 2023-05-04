#ifndef _IDRAWCONFLICTRESOLVER_H_
#define _IDRAWCONFLICTRESOLVER_H_

#include <chrono>
#include <memory>
#include <string>
#include <vector>

#include "global.h"

struct DrawAction {
    ResolverActionType type;
    std::chrono::time_point<std::chrono::system_clock> time;
}

class IDrawConflictResolver {
  public:
    virtual ~IDrawConflictResolver() = default;

    virtual std::vector<DrawAction> resolve(std::vector<DrawAction>) {
    }
};

#endif  // _IDRAWCONFLICTRESOLVER_H_
