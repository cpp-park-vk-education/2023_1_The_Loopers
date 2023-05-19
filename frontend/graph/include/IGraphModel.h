#pragma once

#include "IEdge.h"
#include "IVertex.h"

#include <memory>
#include <set>
#include <string>
#include <vector>

class IEdge;
class IVertex;

class IGraphModel
{
public:
    virtual void FillGraphByEdges(std::string &rawData) = 0;
    virtual std::set<std::shared_ptr<IVertex>> GetUniqueVertexes() const = 0;
    virtual std::vector<std::shared_ptr<IEdge>> GetEdges() const = 0;

private:
    struct NamingsEdge
    {
        std::string source;
        std::string destination;
    };

    std::set<std::shared_ptr<IVertex>> m_uniqueVertexes;
    std::vector<std::shared_ptr<IEdge>> m_edges;

    virtual std::set<std::shared_ptr<IVertex>> MakeUniqueVertexes() = 0;
    virtual void FillUniqueVertexes(std::set<std::shared_ptr<IVertex>>) = 0;
    virtual void FillEdges(const std::vector<NamingsEdge> namings) = 0;
    virtual std::vector<NamingsEdge> ParseRawData(std::string &rawEdgeString) = 0;
};