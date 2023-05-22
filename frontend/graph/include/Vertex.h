#pragma once

#include "IVertex.h"

#include <set>
#include <string>
#include <vector>

namespace inklink::graph
{
// class declaration
class Vertex : public IVertex
{
public:
    struct Velocity
    {
        double xVelocity;
        double yVelocity;
    };

    explicit Vertex(std::string& name) : m_name(name){};

    [[nodiscard]] const std::string& getName() const override;
    [[nodiscard]] Position GetCurrentPosition() const override;
    void SetCurrentPosition(Position) override;
    [[nodiscard]] Velocity GetCurrentVelocity() const;
    void SetCurrentVelocity(Velocity);

private:
    std::string m_name;
    Position m_currentPosition;
    Velocity m_velocity;

private:
    void UpdateVertices(std::vector<Vertex>& vertices, double deltaTime, double attractionForce);
    void ApplyAttractionForce(Vertex& currentVertex, Vertex& otherVertex, double attractionForce);
};
} // namespace inklink::graph
