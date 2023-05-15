#pragma once

#include "idraw_conflict_resolver.h"

namespace inklink::service_simultaneous_access
{
class BlockingDrawResolver final : public IDrawConflictResolver
{
public:
    ~BlockingDrawResolver() final = default;

    // kSelect (and kInsertion as equivalent) should be passed each time => caller should track selections and
    // deselections and remember selections
    std::vector<DrawAction> Resolve(std::vector<DrawAction>) final;
};
} // namespace inklink::service_simultaneous_access
