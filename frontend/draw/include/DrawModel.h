#pragma once

#include "IObject.h"

#include <string>
#include <vector>

namespace {
class DrawView;
}

namespace inklink::draw
{
class DrawModel
{
public:
    std::string SerializeToSend(int actionID, int actionType, int figureID);

protected:
    void addObject(size_t, size_t, std::vector<Point>&);
    void addObject(size_t, Point&, size_t, size_t);
    void addObject(size_t, std::string&, Point&, size_t);

private:
    DrawView* m_view;
    std::vector<ObjectWithAttributes*> m_objects;
    std::string m_filename;

    void ParseToGet(const std::string& message);
};
} // namespace inklink::draw
