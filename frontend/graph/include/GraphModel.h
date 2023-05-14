#pragma once

#include "IEdge.h"
#include "IVertex.h"

#include <string>
#include <vector>

namespace inklink::graph
{
struct Position
{
    std::size_t xPosition;
    std::size_t yPosition;
};

class GraphModel
{
public:
    void addVertexesToModel(std::vector<std::string> vertexes);
    std::vector<IVertex*> getUniqueVertexes();
    std::vector<IEdge*> getEdges();

private:
    void setPositionToVertexes();

    std::vector<std::pair<IVertex*, Position>> m_uniqueVertexes;
    std::vector<IEdge*> m_edges;
};
} // namespace inklink::graph
