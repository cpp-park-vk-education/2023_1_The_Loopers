#pragma once

#include "GraphModel.h"
#include "IEdge.h"

#include <string>
#include <vector>

namespace inklink::graph
{

class IVertex
{
public:
    struct Position
    {
        std::size_t xPosition;
        std::size_t yPosition;
    };

    explicit IVertex(std::string& name) : m_name(name){};

    void calculateForces();
    bool advancePosition();
    [[nodiscard]] std::string getName();
    [[nodiscard]] Position GetCurrentPosition();
    void SetCurrentPosition(Position);

private:
    std::string m_name;
    Position m_currentPosition;
};
} // namespace inklink::graph
