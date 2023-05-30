#pragma once

#include "itext_conflict_resolver.h"

#include <vector>

namespace inklink::service_simultaneous_access
{
// for now just does nothing
class StubTextResolver final : public ITextConflictResolver
{
public:
    ~StubTextResolver() final = default;

    void Resolve(std::vector<TextAction>&) final;
};
} // namespace inklink::service_simultaneous_access
