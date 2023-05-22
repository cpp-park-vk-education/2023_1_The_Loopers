#pragma once

#include <string>
#include <vector>

namespace inklink::graph
{
// class declaration
class IVertex
{
public:
    struct Position
    {
        double xPosition;
        double yPosition;
    };

    ~IVertex();

    [[nodiscard]] virtual const std::string& getName() const = 0;
    [[nodiscard]] virtual Position GetCurrentPosition() const = 0;
    virtual void SetCurrentPosition(Position) = 0;

private:
    std::string m_name;
    Position m_currentPosition;
};
} // namespace inklink::graph
