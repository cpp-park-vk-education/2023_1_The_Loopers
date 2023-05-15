#pragma once

#include "IEdge.h"
#include "IVertex.h"

#include <memory>
#include <string>
#include <vector>

namespace inklink::graph
{
class GraphModel
{
public:
    void addVertexesToGraph(std::vector<std::string> vertexes);
    std::vector<IVertex*> getUniqueVertexes();
    std::vector<IEdge*> getEdges();

private:
    //generates random position for vertex, because edge
    Position GeneratePosition();

    // first vertex in vector used as center vertex
    std::vector<std::shared_ptr<IVertex>> m_uniqueVertexes;
    std::vector<IEdge*> m_edges;
};
} // namespace inklink::graph
