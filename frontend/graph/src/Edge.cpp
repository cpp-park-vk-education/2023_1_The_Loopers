#include "IEdge.h"

namespace inklink::graph
{
const std::shared_ptr<IVertex> IEdge::GetSourceVertex() const
{
    return m_sourceVertex;
}

const std::shared_ptr<IVertex> IEdge::GetDestinationVertex() const
{
    return m_destinationVertex;
}
} // namespace inklink::graph