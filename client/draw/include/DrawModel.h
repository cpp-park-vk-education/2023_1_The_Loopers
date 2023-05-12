#pragma once

#include <string>
#include <vector>

struct Point {
    size_t xPosition;
    size_t yPosition;
};

class DrawModel {
public:
    void deserialize(std::string);

private:
    std::vector<ObjectWithAttributes*> m_objects;
protected:
    void addObject(size_t, size_t, std::vector<Point>);
    void addObject(size_t, Point, size_t, size_t);
    void addObject(size_t, std::string, Point, size_t);
    std::string serialize();
};
