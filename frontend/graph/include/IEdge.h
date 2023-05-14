#pragma once

#include "IVertex.h"

class IEdge{
public:
//    IEdge(IVertex* source, IVertex* destination): m_sourceVertex(source), m_destinationVertex(destination) {};

    const IVertex* getSourceVertex();
    const IVertex* getDestinationVertex();

private:
    IVertex* m_sourceVertex;
    IVertex* m_destinationVertex;
};
