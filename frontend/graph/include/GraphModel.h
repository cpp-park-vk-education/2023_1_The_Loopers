#pragma once

#include "IGraphModel.h"
#include "IVertex.h"

#include <string>
#include <vector>

namespace inklink::graph
{
// forward declaration
class Edge;
class Vertex;
class IEdge;
class IVertex;

// class declaration
class GraphModel : public IGraphModel
{
public:
    void FillGraphByEdges(std::string& rawData) override;
    [[nodiscard]] std::vector<IVertex> GetUniqueVertexes() const override;
    [[nodiscard]] std::vector<IEdge> GetEdges() const override;

private:
    std::vector<IVertex> m_uniqueVertexes; // first vertex in container used as center vertex
    std::vector<IEdge> m_edges;

private:
    [[nodiscard]] std::vector<IVertex> MakeUniqueVertexes() override; // parse edges and find unique vertexes
    void FillUniqueVertexes(std::vector<IVertex>) override;
    void FillEdges(const std::vector<NamingsEdge> namings) override;
    [[nodiscard]] IVertex::Position GeneratePosition(); // generates random position for vertex
    void SetRandomPositions();
};
} // namespace inklink::graph
