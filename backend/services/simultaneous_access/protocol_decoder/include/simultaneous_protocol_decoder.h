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
    ActionType GetActionType(IData*) = 0;
    TextAction GetTextAction(IData*) = 0;
    DrawAction GetDrawAction(IData*) = 0;
    bool IsValid(Idata*) = 0;
};
} // namespace inklink::service_simultaneous_access
