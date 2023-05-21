#include "stub_text_resolver.h"

#include <unordered_map>
#include <vector>

namespace inklink::service_simultaneous_access
{

void StubTextResolver::Resolve(std::vector<TextAction>& actions)
{
    for (const auto& action : actions)
    {
        m_history.push_back(action);
    }
}

} // namespace inklink::service_simultaneous_access
