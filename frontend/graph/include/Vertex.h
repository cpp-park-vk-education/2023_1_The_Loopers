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
    void SetCurrentPosition(Position newPosition) override;
    [[nodiscard]] Velocity GetCurrentVelocity() const;
    void SetCurrentVelocity(Velocity newVelocity);
    [[nodiscard]] double GetMass() const;
    void SetMass(double newMass);

private:
    std::string m_name;
    Position m_currentPosition;
    Velocity m_velocity;
    double m_mass;

private:
    // maybe should be public
    void UpdateVertices(std::vector<Vertex>& vertices, double deltaTime, double attractionForce);
    void ApplyAttractionForce(Vertex& firstVertex, Vertex& secondVertex, double attractionForce);
};
} // namespace inklink::graph
