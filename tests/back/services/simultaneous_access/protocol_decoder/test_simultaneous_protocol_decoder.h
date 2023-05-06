#ifndef _SIMULTANEOUSPROTOCOLDECODER_H_
#define _SIMULTANEOUSPROTOCOLDECODER_H_

#include <memory>
#include <string>

#include "idata.h"
#include "idraw_conflict_resolver.h"
#include "itext_conflict_resolver.h"

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
};

#endif  // _SIMULTANEOUSPROTOCOLDECODER_H_
