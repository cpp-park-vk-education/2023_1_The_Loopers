#pragma once

#include "IEdge.h"
#include "IGraphModel.h"
#include "IVertex.h"

#include <memory>
#include <set>
#include <string>
#include <vector>

namespace inklink::graph
{

class IEdge;
class IVertex;

class GraphModel : public IGraphModel
{
public:
    void FillGraphByEdges(std::string& rawData) override;
    [[nodiscard]] std::set<std::shared_ptr<IVertex>> GetUniqueVertexes() const override;
    [[nodiscard]] std::vector<std::shared_ptr<IEdge>> GetEdges() const override;

private:
    std::set<std::shared_ptr<IVertex>> m_uniqueVertexes; // first vertex in container used as center vertex
    std::vector<std::shared_ptr<IEdge>> m_edges;

    [[nodiscard]] std::set<std::shared_ptr<IVertex>> MakeUniqueVertexes() override; // parse edges and find unique vertexes
    void FillUniqueVertexes(std::set<std::shared_ptr<IVertex>>) override;
    void FillEdges(const std::vector<NamingsEdge> namings) override;
    [[nodiscard]] IVertex::Position GeneratePosition(); // generates random position for vertex
    void SetRandomPositions();
    [[nodiscard]] std::vector<NamingsEdge> ParseRawData(std::string& rawEdgeString);
};
} // namespace inklink::graph
