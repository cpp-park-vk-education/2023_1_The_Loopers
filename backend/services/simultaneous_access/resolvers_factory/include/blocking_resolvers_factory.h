#pragma once

#include "iresolvers_factory.h"

namespace inklink::service_simultaneous_access
{
class BlockingResolversFactory final : public IResolversFactory
{
public:
    ~BlockingResolversFactory() override = default;

    [[nodiscard]] ITextConflictResolver* GetTextConflictResolver() const override;
    [[nodiscard]] IDrawConflictResolver* GetDrawConflictResolver() const override;
};
} // namespace inklink::service_simultaneous_access
