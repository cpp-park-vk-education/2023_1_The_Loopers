#ifndef _IRESOLVERSFACTORY_H_
#define _IRESOLVERSFACTORY_H_

#include "IDrawConflictResolver.h"
#include "ITextConflictResolver.h"

class IResolversFactory {
  public:
    virtual ~IResolversFactory() = default;

    virtual ITextConflictResolver* GetTextConflictResolver() {
    }
    virtual IDrawConflictResolver* GetDrawConflictResolver() {
    }
};

#endif  // _IRESOLVERSFACTORY_H_
