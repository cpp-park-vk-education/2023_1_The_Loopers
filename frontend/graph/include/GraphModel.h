#pragma once

#include "IEdge.h"
#include "IVertex.h"

#include <memory>
#include <set>
#include <string>
#include <vector>

namespace inklink::graph
{
// DB -[edges string]-> Fill Graph <- {ParseData, FillEdges, MakeUniqueVertexes, FillUniqueVertexes, GeneratePosition}
class GraphModel
{
public:
    void FillGraphByEdges(const std::vector<std::string>& vertexes);
    std::vector<std::shared_ptr<IVertex>> GetUniqueVertexes();
    std::vector<std::shared_ptr<IEdge>> GetEdges();

//    void AddEdge(NamingsEdge);

private:
    struct NamingsEdge
    {
        std::string source;
        std::string destination;
    };

    Position GeneratePosition();                                // generates random position for vertex
    std::set<std::shared_ptr<IVertex>> MakeUniqueVertexes(); // parse edges and find unique vertexes
    void FillUniqueVertexes(std::set<std::shared_ptr<IVertex>>);
    void FillEdges(std::vector<NamingsEdge>);
    void SetRandomPositions();
    std::vector<NamingsEdge> ParseRawData(std::string& rawString); // parse raw string and makes edges with vertex namings

    // first vertex in vector used as center vertex
    std::set<std::shared_ptr<IVertex>> m_uniqueVertexes;
    std::vector<std::shared_ptr<IEdge>> m_edges;
};
} // namespace inklink::graph
