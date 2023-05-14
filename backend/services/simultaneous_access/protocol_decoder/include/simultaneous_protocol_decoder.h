#pragma once

#include "idata.h"
#include "idraw_conflict_resolver.h"
#include "itext_conflict_resolver.h"

#include <memory>
#include <string>

namespace inklink::service_simultaneous_access
{
class SimultaneousProtocolDecoder
{
public:
    ActionType GetActionType(IData*)
    {
    }
    TextAction GetTextAction(IData*)
    {
    }
    DrawAction GetDrawAction(IData*)
    {
    }
    bool IsValid(Idata*)
    {
    }
};
} // namespace inklink::service_simultaneous_access
