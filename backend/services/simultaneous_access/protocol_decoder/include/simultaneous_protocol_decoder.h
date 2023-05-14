#pragma once

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
    ActionType GetActionType(IData*) = 0;
    TextAction GetTextAction(IData*) = 0;
    DrawAction GetDrawAction(IData*) = 0;
    bool IsValid(Idata*) = 0;
};
} // namespace inklink::service_simultaneous_access
