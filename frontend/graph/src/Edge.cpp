#include "Edge.h"

#include <memory>

namespace inklink::graph
{
const std::shared_ptr<IVertex> Edge::GetSourceVertex() const
{
    return m_sourceVertex;
}

const std::shared_ptr<IVertex> Edge::GetDestinationVertex() const
{
    return m_destinationVertex;
}
} // namespace inklink::graph