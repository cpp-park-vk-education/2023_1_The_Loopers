#pragma once

#include "IEdge.h"
#include "IVertex.h"

#include <string>
#include <vector>

namespace inklink::graph
{

struct Position
{
    std::size_t xPosition;
    std::size_t yPosition;
};

class Vertex : public IVertex
{
public:
    void calculateForces();
    bool advancePosition();
    std::string getName();

    Position getCurrentPosition();
    Position getNewPosition();
    void setCurrentPosition(Position);
    void setNewPosition(Position);

private:
    std::string m_name;
    Position m_currentPosition;
};
} // namespace inklink::graph
