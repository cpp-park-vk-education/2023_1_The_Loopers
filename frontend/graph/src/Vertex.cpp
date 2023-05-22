#include "Vertex.h"

namespace
{
constexpr double k_damping = 0.9;
}

namespace inklink::graph
{
void Vertex::UpdateVertices(std::vector<Vertex>& vertices, double deltaTime, double attractionForce)
{
    for (size_t i = 0; i < vertices.size(); i++)
    {
        for (size_t j = i + 1; j < vertices.size(); j++)
        {
            ApplyAttractionForce(vertices.at(i), vertices.at(j), attractionForce);
        }
    }

    for (const auto& vertex : vertices)
    {
        auto velocity = GetCurrentVelocity();
        velocity.xVelocity *= k_damping;
        velocity.yVelocity *= k_damping;
        SetCurrentVelocity(velocity);

        auto position = GetCurrentPosition();
        position.xPosition += position.xPosition * deltaTime;
        position.yPosition += position.yPosition * deltaTime;
        SetCurrentPosition(position);
    }
}
} // namespace inklink::graph
