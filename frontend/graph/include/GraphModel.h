#pragma once

#include "IEdge.h"
#include "IVertex.h"

#include <memory>
#include <string>
#include <vector>

namespace inklink::graph
{
enum positionLimits
{
    kPosition = 300
};

struct NamingsEdge
{
    std::string source;
    std::string destination;
};

// DB -[edges string]-> Fill Graph <- {ParseData, FillEdges, MakeUniqueVertexes, FillUniqueVertexes, GeneratePosition}
class GraphModel
{
public:
    void FillGraphByEdges(const std::vector<std::string>& vertexes);
    std::vector<std::shared_ptr<IVertex>> GetUniqueVertexes();
    std::vector<std::shared_ptr<IEdge>> GetEdges();

    void AddEdge(NamingsEdge);

private:
    Position GeneratePosition();                                // generates random position for vertex
    std::vector<std::shared_ptr<IVertex>> MakeUniqueVertexes(); // parse edges and find unique vertexes
    void FillUniqueVertexes(std::shared_ptr<IVertex>);
    void FillEdges(std::vector<NamingsEdge>);
    void SetPosition ();
    std::vector<NamingsEdge> ParseRawData(std::string& rawString); // parse raw string and makes edges with vertex namings

    // first vertex in vector used as center vertex
    std::vector<std::shared_ptr<IVertex>> m_uniqueVertexes;
    std::vector<std::shared_ptr<IEdge>> m_edges;
};
} // namespace inklink::graph
