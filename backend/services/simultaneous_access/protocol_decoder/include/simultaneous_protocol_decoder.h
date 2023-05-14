#pragma once

#include "idraw_conflict_resolver.h"
#include "inklink_global.h"
#include "itext_conflict_resolver.h"

#include <memory>
#include <string>


namespace inklink::serializer
{
class IData;
}

namespace inklink::service_simultaneous_access
{
class SimultaneousProtocolDecoder
{
    using IData = serializer::IData;

public:
    ActionType GetActionType(IData*);
    TextAction GetTextAction(IData*);
    DrawAction GetDrawAction(IData*);
    bool IsValid(IData*);
};
} // namespace inklink::service_simultaneous_access
