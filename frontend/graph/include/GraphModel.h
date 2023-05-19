#pragma once

#include "IEdge.h"
#include "IVertex.h"

#include <memory>
#include <set>
#include <string>
#include <vector>

namespace inklink::graph
{
class GraphModel
{
public:
    void FillGraphByEdges(std::string& rawData);
    std::set<std::shared_ptr<IVertex>> GetUniqueVertexes() const;
    std::vector<std::shared_ptr<IEdge>> GetEdges() const;

private:
    struct NamingsEdge
    {
        std::string source;
        std::string destination;
    };

    Position GeneratePosition();                             // generates random position for vertex
    std::set<std::shared_ptr<IVertex>> MakeUniqueVertexes(); // parse edges and find unique vertexes
    void FillUniqueVertexes(std::set<std::shared_ptr<IVertex>>);
    void FillEdges(const std::vector<NamingsEdge> namings);
    void SetRandomPositions();
    std::vector<NamingsEdge> ParseRawData(std::string& rawEdgeString); // parse raw string and makes
                                                                       // edges with vertex namings

    // first vertex in vector used as center vertex
    std::set<std::shared_ptr<IVertex>> m_uniqueVertexes;
    std::vector<std::shared_ptr<IEdge>> m_edges;
};
} // namespace inklink::graph
