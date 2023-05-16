#pragma once

#include "IVertex.h"

#include <memory>

namespace inklink::graph
{
class IEdge
{
public:
    //    IEdge(IVertex* source, IVertex* destination): m_sourceVertex(source), m_destinationVertex(destination) {};

    const std::shared_ptr<IVertex> GetSourceVertex() const;
    const std::shared_ptr<IVertex> GetDestinationVertex() const;

private:
    IVertex* m_sourceVertex;
    IVertex* m_destinationVertex;
};
} // namespace inklink::graph
