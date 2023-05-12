#pragma once

#include "GraphModel.h"
#include "IEdge.h"

#include <vector>

class IVertex{
public:
    void addEdge(IEdge* edge);
    void calculateForces();
    bool advancePosition();
    std::vector<IEdge*> getEdges();
    std::string getName();
    Position getCurrentPosition();
    Position getNewPosition();
    void setCurrentPosition(Position);
    void setNewPosition(Position);

private:
    std::string m_name;
    std::vector<IEdge*> m_edgeList;
    Position m_currentPosition;
    Position m_newPosition;
};
