#pragma once

#include "IEdge.h"
#include "IVertex.h"

#include <string>
#include <vector>

namespace inklink::graph
{
class GraphModel
{
public:
    void addVertexesToModel(std::vector<std::string> vertexes);
    std::vector<IVertex*> getUniqueVertexes();
    std::vector<IEdge*> getEdges();

private:
    void setPositionToVertexes(Position position);

    std::vector<IVertex*> m_uniqueVertexes;
    std::vector<IEdge*> m_edges;
};
} // namespace inklink::graph
