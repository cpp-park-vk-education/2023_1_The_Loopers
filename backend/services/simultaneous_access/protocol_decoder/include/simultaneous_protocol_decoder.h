#pragma once

#include "idata.h"
#include "idraw_conflict_resolver.h"
#include "itext_conflict_resolver.h"

#include <memory>
#include <string>


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
