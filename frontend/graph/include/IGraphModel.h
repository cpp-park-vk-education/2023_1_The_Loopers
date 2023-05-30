#pragma once

#include <memory>
#include <set>
#include <string>
#include <utility>
#include <vector>

namespace inklink::graph
{
// forward declaration
class IEdge;
class IVertex;

// class declaration
class IGraphModel
{
public:
    struct NamingsEdge
    {
        NamingsEdge(std::string source_, std::string destination_)
                : source(std::move(source_)), destination(std::move(destination_)){};

        std::string source;
        std::string destination;
    };

    virtual void FillGraphByEdges(std::string& rawData) = 0;
    [[nodiscard]] virtual std::vector<IVertex> GetUniqueVertexes() const = 0;
    [[nodiscard]] virtual std::vector<IEdge> GetEdges() const = 0;

private:
    std::vector<IVertex> m_uniqueVertexes;
    std::vector<IEdge> m_edges;

private:
    [[nodiscard]] virtual std::vector<IVertex> MakeUniqueVertexes() = 0;
    virtual void FillUniqueVertexes(std::vector<IVertex>) = 0;
    virtual void FillEdges(const std::vector<NamingsEdge> namings) = 0;
};
} // namespace inklink::graph
