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
    std::string SerializeToSend(int actionID, int actionType, int figureID);

protected:
    void addObject(size_t, size_t, std::vector<Point>&);
    void addObject(size_t, Point&, size_t, size_t);
    void addObject(size_t, std::string&, Point&, size_t);

private:
    std::vector<ObjectWithAttributes*> m_objects;
    std::string m_filename;
};
} // namespace inklink::draw
