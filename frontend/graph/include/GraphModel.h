#pragma once

#include "IEdge.h"
#include "IVertex.h"

#include <memory>
#include <string>
#include <vector>

namespace inklink::graph
{

enum positionLimits{
    kPosition = 300
};

class GraphModel
{
public:
    void AddVertexesToGraph(const std::vector<std::string>& vertexes);
    std::vector<std::shared_ptr<IVertex>> GetUniqueVertexes();
    std::vector<std::shared_ptr<IEdge>> GetEdges();

private:
    //generates random position for vertex, because edge
    Position GeneratePosition();

    // first vertex in vector used as center vertex
    std::vector<std::shared_ptr<IVertex>> m_uniqueVertexes;
    std::vector<std::shared_ptr<IEdge>> m_edges;
};
} // namespace inklink::graph
