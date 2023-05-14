#pragma once

#include "idraw_conflict_resolver.h"
#include "itext_conflict_resolver.h"

class IResolversFactory
{
public:
    virtual ~IResolversFactory() = default;

    virtual ITextConflictResolver* GetTextConflictResolver()
    {
    }
    virtual IDrawConflictResolver* GetDrawConflictResolver()
    {
    }
};
