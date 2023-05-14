#pragma once

#include "IObject.h"

#include <string>
#include <vector>

struct Point {
    std::size_t xPosition;
    std::size_t yPosition;
};

class DrawModel {
public:
    void deserialize(std::string);

protected:
    void addObject(size_t, size_t, std::vector<Point>);
    void addObject(size_t, Point, size_t, size_t);
    void addObject(size_t, std::string, Point, size_t);
    std::string serialize();

private:
    std::vector<ObjectWithAttributes*> m_objects;
};
