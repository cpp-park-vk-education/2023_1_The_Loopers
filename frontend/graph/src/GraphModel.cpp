#include "GraphModel.h"

#include <iostream>
#include <memory>
#include <random>
#include <set>
#include <sstream>
#include <string>

constexpr std::size_t kPosition = 300;

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
    std::uniform_int_distribution<> dis(-kPosition, kPosition);

    Position temp;
    temp.yPosition = dis(gen);
    temp.xPosition = dis(gen);
    return temp;
}

std::set<std::shared_ptr<IVertex>> GraphModel::MakeUniqueVertexes()
{
    std::set<std::shared_ptr<IVertex>> uniqueVertexes;

    for (const auto& edge : m_edges)
    {
        uniqueVertexes.insert(edge->GetSourceVertex());
        uniqueVertexes.insert(edge->GetDestinationVertex());
    }

    return uniqueVertexes;
}

void GraphModel::FillUniqueVertexes(std::set<std::shared_ptr<IVertex>> uniqueVertexes)
{
    m_uniqueVertexes = std::move(uniqueVertexes);
}

void GraphModel::FillEdges(std::vector<NamingsEdge> namings)
{
    for (auto rawEdge : namings)
    {
        std::shared_ptr<IVertex> sourceVertex = std::make_shared<IVertex>(rawEdge.source);
        std::shared_ptr<IVertex> destinationVertex = std::make_shared<IVertex>(rawEdge.destination);
        IEdge(sourceVertex, destinationVertex);

        m_edges.push_back(IEdge);
    }
}

void GraphModel::SetRandomPositions()
{
    for (auto vertex : m_uniqueVertexes)
    {
        auto vertexPosition = GeneratePosition();
        vertex->SetCurrentPosition(vertexPosition);
    }
}

std::vector<GraphModel::NamingsEdge> GraphModel::ParseRawData(std::string& rawEdgeString)
{
    std::stringstream stringToParse(rawEdgeString);
    std::string singleEdge;
    std::vector<NamingsEdge> namings;

    while (stringToParse >> singleEdge)
    {
        NamingsEdge edge;
        auto spacePosition = singleEdge.find('\t');
        auto source = singleEdge.substr(0, spacePosition);
        auto destination = singleEdge.substr(spacePosition + 1);
        edge.source = source;
        edge.destination = destination;
        namings.push_back(edge);
    }

    return namings;
}
} // namespace inklink::graph
