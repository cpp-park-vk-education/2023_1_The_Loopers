#pragma once

#include "IVertex.h"

#include <memory>

namespace inklink::graph
{
class IEdge;
class IVertex;

class Edge : public IEdge
{
public:
    Edge(std::shared_ptr<IVertex> source, std::shared_ptr<IVertex> destination)
            : m_sourceVertex(source), m_destinationVertex(destination){};

    [[nodiscard]] const std::shared_ptr<IVertex> GetSourceVertex() const override;
    [[nodiscard]] const std::shared_ptr<IVertex> GetDestinationVertex() const override;

private:
    std::shared_ptr<IVertex> m_sourceVertex;
    std::shared_ptr<IVertex> m_destinationVertex;
};
} // namespace inklink::graph
