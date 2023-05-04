#ifndef _SIMULTANEOUSPROTOCOLDECODER_H_
#define _SIMULTANEOUSPROTOCOLDECODER_H_

#include <memory>
#include <string>

#include "IData.h"
#include "IDrawConflictResolver.h"
#include "ITextConflictResolver.h"

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

#endif  // _SIMULTANEOUSPROTOCOLDECODER_H_
