#pragma once

namespace inklink::service_simultaneous_access
{
class ITextConflictResolver;
class IDrawConflictResolver;
class IResolversFactory
{
public:
    virtual ~IResolversFactory() = default;

    [[nodiscard]] virtual ITextConflictResolver* GetTextConflictResolver() const = 0;
    [[nodiscard]] virtual IDrawConflictResolver* GetDrawConflictResolver() const = 0;
};
} // namespace inklink::service_simultaneous_access
