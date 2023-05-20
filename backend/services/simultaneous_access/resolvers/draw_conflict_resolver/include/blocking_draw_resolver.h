#pragma once

#include "idraw_conflict_resolver.h"

#include <unordered_map>
#include <vector>

namespace inklink::service_simultaneous_access
{
class BlockingDrawResolver final : public IDrawConflictResolver
{
public:
    ~BlockingDrawResolver() final = default;

    // kSelect should be passed each time => caller should track selections and
    // deselections and remember selections
    void Resolve(std::vector<DrawAction>&) const final;

private:
    /// @brief stores selected figures and users by whom it was selected
    mutable std::unordered_map<std::string, Endpoint> m_userBySelectedFigure;
};
} // namespace inklink::service_simultaneous_access
