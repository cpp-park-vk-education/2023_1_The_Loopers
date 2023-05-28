//#include "GraphModel.h"
//
//#include "Edge.h"
//#include "Vertex.h"
//
//#include <iostream>
//#include <memory>
//#include <random>
//#include <set>
//#include <sstream>
//#include <string>
//
//using namespace inklink::graph;
//
//namespace
//{
//constexpr int kPosition = 300;
//
//// should be used serializer-deserializer
//[[nodiscard]] auto ParseRawData(const std::string& rawEdgeString)
//{
//    std::stringstream stringToParse(rawEdgeString);
//    std::string singleEdge{};
//    std::vector<IGraphModel::NamingsEdge> namings{};
//
//    while (stringToParse >> singleEdge)
//    {
//        const auto spacePosition = singleEdge.find('\t');
//        const auto source = singleEdge.substr(0, spacePosition);
//        const auto destination = singleEdge.substr(spacePosition + 1);
//        namings.emplace_back(source, destination);
//    }
//
//    return namings;
//}
//} // namespace
//
//namespace inklink::graph
//{
//void GraphModel::FillGraphByEdges(std::string& rawData)
//{
//    auto namingsOfEdges = ParseRawData(rawData);
//    FillEdges(namingsOfEdges);
//    auto uniqueVertexes = MakeUniqueVertexes();
//    FillUniqueVertexes(uniqueVertexes);
//    SetRandomPositions();
//}
//
//std::vector<IVertex> GraphModel::GetUniqueVertexes() const
//{
//    return m_uniqueVertexes;
//}
//
//std::vector<IEdge> GraphModel::GetEdges() const
//{
//    return m_edges;
//}
//
//// also would be used for shuffle in 'view'
//IVertex::Position GraphModel::GeneratePosition()
//{
//    std::random_device seed;
//    std::mt19937 gen(seed());
//    std::uniform_int_distribution<> dis(-kPosition, kPosition);
//
//    IVertex::Position temp;
//    temp.yPosition = dis(gen);
//    temp.xPosition = dis(gen);
//    return temp;
//}
//
//std::vector<IVertex> GraphModel::MakeUniqueVertexes()
//{
//    std::vector<IVertex> uniqueVertexes;
//
//    for (const auto& edge : m_edges)
//    {
//        // currently don't know how
//    }
//
//    return uniqueVertexes;
//}
//
//void GraphModel::FillUniqueVertexes(std::vector<IVertex> uniqueVertexes)
//{
//    m_uniqueVertexes = std::move(uniqueVertexes);
//}
//
//void GraphModel::FillEdges(const std::vector<NamingsEdge> namings)
//{
//    for (auto rawEdge : namings)
//    {
//        // don't know how
//    }
//}
//
//void GraphModel::SetRandomPositions()
//{
//    for (auto& vertex : m_uniqueVertexes)
//    {
//        auto vertexPosition = GeneratePosition();
//        vertex.SetCurrentPosition(vertexPosition);
//    }
//}
//
//} // namespace inklink::graph
