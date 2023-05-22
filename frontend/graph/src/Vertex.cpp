#include "Vertex.h"

namespace
{
constexpr double k_damping = 0.9;
constexpr double k_maxDistance = 100;
}

namespace inklink::graph
{
void Vertex::ApplyAttractionForce(Vertex& firstVertex, Vertex& secondVertex, double attractionForce) {
    auto dist = distance(firstVertex, secondVertex);
    auto force = attractionForce * (dist - k_maxDistance);
    auto dx = (secondVertex.GetCurrentPosition().xPosition - firstVertex.GetCurrentPosition().xPosition) / dist;
    auto dy = (secondVertex.GetCurrentPosition().yPosition - firstVertex.GetCurrentPosition().yPosition) / dist;

    auto firstVertexMass = firstVertex.GetMass();
    auto firstVertexVelocity = firstVertex.GetCurrentVelocity();
    firstVertexVelocity.xVelocity += force * dx / firstVertexMass;
    firstVertexVelocity.yVelocity += force * dy / firstVertexMass;
    firstVertex.SetCurrentVelocity(firstVertexVelocity);

    auto secondVertexMass = secondVertex.GetMass();
    auto secondVertexVelocity = secondVertex.GetCurrentVelocity();
    secondVertexVelocity.xVelocity += force * dx / secondVertexMass;
    secondVertexVelocity.yVelocity += force * dy / secondVertexMass;
    secondVertex.SetCurrentVelocity(secondVertexVelocity);
}

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
