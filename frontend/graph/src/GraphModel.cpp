#include "GraphModel.h"

#include <memory>
#include <string>

namespace inklink::graph
{
// TODO should parse raw data to edges and unique vertexes, then add them to graph
void GraphModel::addVertexesToGraph(std::vector<std::string> vertexes)
{
    for (std::string vertexName : vertexes)
    {
        auto vertexPosition = GeneratePosition();

        std::shared_ptr<IVertex> newVertex = std::make_shared<IVertex>(vertexName, vertexPosition);
        m_uniqueVertexes.push_back(newVertex);
    }
}

std::vector<std::shared_ptr<IVertex>> GraphModel::getUniqueVertexes()
{
    return m_uniqueVertexes;
}

std::vector<IEdge*> GraphModel::getEdges()
{
}

Position GraphModel::GeneratePosition()
{
}
} // namespace inklink::graph