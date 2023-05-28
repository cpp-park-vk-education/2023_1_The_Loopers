#include "blocking_resolvers_factory.h"

#include "blocking_draw_resolver.h"
#include "stub_text_resolver.h"

namespace inklink::service_simultaneous_access
{

ITextConflictResolver* BlockingResolversFactory::GetTextConflictResolver() const
{
    return new StubTextResolver;
}

IDrawConflictResolver* BlockingResolversFactory::GetDrawConflictResolver() const
{
    return new BlockingDrawResolver;
}

} // namespace inklink::service_simultaneous_access
