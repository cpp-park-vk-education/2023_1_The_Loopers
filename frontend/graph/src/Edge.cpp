#include "IEdge.h"

namespace inklink::graph
{
const std::shared_ptr<IVertex> IEdge::GetDestinationVertex() const
{
    return m_destinationVertex;
}
}