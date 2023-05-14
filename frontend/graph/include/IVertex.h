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
//    Position m_newPosition;
};
} // namespace inklink::graph
