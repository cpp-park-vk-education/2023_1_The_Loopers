#pragma once

#include "idraw_conflict_resolver.h"

#include <unordered_map>
#include <vector>

namespace inklink::service_simultaneous_access
{
class BlockingDrawResolver final : public IDrawConflictResolver
{
public:
    explicit BlockingDrawResolver(std::size_t maxFigureId) noexcept;
    ~BlockingDrawResolver() final = default;

    // kSelect should be passed each time => caller should track selections and
    // deselections and remember selections
    void Resolve(std::vector<DrawAction>&) final;

private:
    /// @brief stores selected figures and users by whom it was selected
    mutable std::unordered_map<std::string, Endpoint> m_userBySelectedFigure;
    std::size_t m_maxFigureId;
};
} // namespace inklink::service_simultaneous_access
