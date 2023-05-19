#pragma once

#include "IVertex.h"

#include <memory>

namespace inklink::graph
{
class IVertex;

class IEdge
{
public:
    [[nodiscard]] virtual const std::shared_ptr<IVertex> GetSourceVertex() const = 0;
    [[nodiscard]] virtual const std::shared_ptr<IVertex> GetDestinationVertex() const = 0;

private:
    std::shared_ptr<IVertex> m_sourceVertex;
    std::shared_ptr<IVertex> m_destinationVertex;
};
} // namespace inklink::graph
