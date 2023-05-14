#pragma once

#include "idraw_conflict_resolver.h"
#include "itext_conflict_resolver.h"

namespace inklink::service_simultaneous_access
{
class IResolversFactory
{
public:
    virtual ~IResolversFactory() = default;

    virtual ITextConflictResolver* GetTextConflictResolver() = 0;
    virtual IDrawConflictResolver* GetDrawConflictResolver() = 0;
};
} // namespace inklink::service_simultaneous_access
