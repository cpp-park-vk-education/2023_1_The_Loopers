#include "GraphModel.h"

#include <memory>
#include <random>
#include <string>

namespace inklink::graph
{
enum{
    kPosition = 300
};

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

std::vector<std::shared_ptr<IEdge>> GraphModel::getEdges()
{
    return m_edges;
}

// also would be used for shuffle in 'view'
Position GraphModel::GeneratePosition()
{
    std::random_device seed;
    std::mt19937 gen(seed());
    std::uniform_int_distribution<> dis(-kPosition, kPosition);

    Position temp;
    temp.yPosition = dis(gen);
    temp.xPosition = dis(gen);
    return temp;
}
} // namespace inklink::graph
