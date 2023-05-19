#pragma once

#include "IVertex.h"

namespace inklink::graph
{
class IEdge
{
public:
    //    IEdge(IVertex* source, IVertex* destination): m_sourceVertex(source), m_destinationVertex(destination) {};

    [[nodiscard]] const std::shared_ptr<IVertex> GetSourceVertex() const;
    [[nodiscard]] const std::shared_ptr<IVertex> GetDestinationVertex() const;

private:
    std::shared_ptr<IVertex> m_sourceVertex;
    std::shared_ptr<IVertex> m_destinationVertex;
};
} // namespace inklink::graph
