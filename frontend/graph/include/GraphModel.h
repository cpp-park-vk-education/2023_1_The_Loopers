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
class GraphModel : public IGraphModel
{
public:
    void FillGraphByEdges(std::string& rawData) override;
    std::set<std::shared_ptr<IVertex>> GetUniqueVertexes() const override;
    std::vector<std::shared_ptr<IEdge>> GetEdges() const override;

private:
    struct NamingsEdge
    {
        std::string source;
        std::string destination;
    };

    // first vertex in vector used as center vertex
    std::set<std::shared_ptr<IVertex>> m_uniqueVertexes;
    std::vector<std::shared_ptr<IEdge>> m_edges;

    Position GeneratePosition();                             // generates random position for vertex
    std::set<std::shared_ptr<IVertex>> MakeUniqueVertexes() override; // parse edges and find unique vertexes
    void FillUniqueVertexes(std::set<std::shared_ptr<IVertex>>) override;
    void FillEdges(const std::vector<NamingsEdge> namings) override;
    void SetRandomPositions() ;
    std::vector<NamingsEdge> ParseRawData(std::string& rawEdgeString) override; // parse raw string and makes
                                                                       // edges with vertex namings
};
} // namespace inklink::graph
