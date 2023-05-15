#pragma once

#include "IObject.h"

#include <string>
#include <vector>

namespace inklink::draw
{
class DrawModel
{
public:
    void deserialize(std::string);

protected:
    void addObject(size_t, size_t, std::vector<Point>&);
    void addObject(size_t, Point&, size_t, size_t);
    void addObject(size_t, std::string&, Point&, size_t);
    std::string serialize();

private:
    std::vector<ObjectWithAttributes*> m_objects;
};
} // namespace inklink::draw
