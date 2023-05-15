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
    std::vector<std::shared_ptr<IVertex>> getUniqueVertexes();
    std::vector<std::shared_ptr<IEdge>> getEdges();

private:
    //generates random position for vertex, because edge
    Position GeneratePosition();

    // first vertex in vector used as center vertex
    std::vector<std::shared_ptr<IVertex>> m_uniqueVertexes;
    std::vector<std::shared_ptr<IEdge>> m_edges;
};
} // namespace inklink::graph
