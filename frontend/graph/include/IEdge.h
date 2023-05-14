#pragma once

#include "IVertex.h"

namespace inklink::graph
{
class IEdge
{
public:
    //    IEdge(IVertex* source, IVertex* destination): m_sourceVertex(source), m_destinationVertex(destination) {};

    const IVertex* getSourceVertex();
    const IVertex* getDestinationVertex();

private:
    IVertex* m_sourceVertex;
    IVertex* m_destinationVertex;
};
} // namespace inklink::graph
