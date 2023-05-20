#pragma once

#include "iresolvers_factory.h"

namespace inklink::service_simultaneous_access
{
class BlockingResolversFactory final : public IResolversFactory
{
public:
    ~BlockingResolversFactory() final = default;

    [[nodiscard]] ITextConflictResolver* GetTextConflictResolver() const final;
    [[nodiscard]] IDrawConflictResolver* GetDrawConflictResolver() const final;
};
} // namespace inklink::service_simultaneous_access
