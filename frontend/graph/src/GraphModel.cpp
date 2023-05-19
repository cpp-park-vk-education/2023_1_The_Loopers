#include "GraphModel.h"

#include <iostream>
#include <memory>
#include <random>
#include <set>
#include <sstream>
#include <string>

constexpr int kPosition = 300;

using namespace inklink::graph;

namespace
{
[[nodiscard]] std::vector<IGraphModel::NamingsEdge> ParseRawData(std::string& rawEdgeString)
{
    std::stringstream stringToParse(rawEdgeString);
    std::string singleEdge;
    std::vector<IGraphModel::NamingsEdge> namings;

    while (stringToParse >> singleEdge)
    {
        auto spacePosition = singleEdge.find('\t');
        auto source = singleEdge.substr(0, spacePosition);
        auto destination = singleEdge.substr(spacePosition + 1);
        namings.emplace_back(source, destination);
    }

    return namings;
}
} // namespace

namespace inklink::graph
{
void GraphModel::FillGraphByEdges(std::string& rawData)
{
    auto namingsOfEdges = ParseRawData(rawData);
    FillEdges(namingsOfEdges);
    auto uniqueVertexes = MakeUniqueVertexes();
    FillUniqueVertexes(uniqueVertexes);
    SetRandomPositions();
}

std::set<std::shared_ptr<IVertex>> GraphModel::GetUniqueVertexes() const
{
    return m_uniqueVertexes;
}

std::vector<std::shared_ptr<IEdge>> GraphModel::GetEdges() const
{
    return m_edges;
}

// also would be used for shuffle in 'view'
IVertex::Position GraphModel::GeneratePosition()
{
    std::random_device seed;
    std::mt19937 gen(seed());
    std::uniform_int_distribution<> dis(-kPosition, kPosition);

    IVertex::Position temp;
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

void GraphModel::FillEdges(const std::vector<NamingsEdge> namings)
{
    for (auto rawEdge : namings)
    {
        auto sourceVertex = std::make_shared<IVertex>(rawEdge.source);
        auto destinationVertex = std::make_shared<IVertex>(rawEdge.destination);

        m_edges.emplace_back(std::make_shared<IEdge>(sourceVertex, destinationVertex));
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

} // namespace inklink::graph
