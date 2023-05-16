#include "GraphModel.h"

#include <memory>
#include <random>
#include <set>
#include <string>

namespace inklink::graph
{
// TODO should parse raw data to edges and unique vertexes, then add them to graph
void GraphModel::FillGraphByEdges(const std::vector<std::string>& vertexes)
{
    for (std::string vertexName : vertexes)
    {
        auto vertexPosition = GeneratePosition();

        std::shared_ptr<IVertex> newVertex = std::make_shared<IVertex>(vertexName, vertexPosition);
        m_uniqueVertexes.push_back(std::move(newVertex));
    }
}

std::vector<std::shared_ptr<IVertex>> GraphModel::GetUniqueVertexes()
{
    return m_uniqueVertexes;
}

std::vector<std::shared_ptr<IEdge>> GraphModel::GetEdges()
{
    return m_edges;
}

// also would be used for shuffle in 'view'
Position GraphModel::GeneratePosition()
{
    std::random_device seed;
    std::mt19937 gen(seed());
    std::uniform_int_distribution<> dis(-positionLimits::kPosition, positionLimits::kPosition);

    Position temp;
    temp.yPosition = dis(gen);
    temp.xPosition = dis(gen);
    return temp;
}

std::set<std::shared_ptr<IVertex>> GraphModel::MakeUniqueVertexes()
{
    std::set<std::shared_ptr<IVertex>> uniqueVertexes;

    for (auto edge : m_edges)
    {
        uniqueVertexes.insert(edge->GetSourceVertex());
        uniqueVertexes.insert(edge->GetDestinationVertex());
    }

    return uniqueVertexes;
}
} // namespace inklink::graph
