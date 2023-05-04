#pragma once

#include <string>
#include <vector>

struct Point {
    size_t xPosition;
    size_t yPosition;
};

class ObjectWithAttributes {
public:
    virtual size_t getID() = 0;
    virtual void setID(std::size_t ID) = 0;
    virtual std::string serialize() = 0;
};

class TextBox : public ObjectWithAttributes {
public:
    size_t getID() override;
    void setID(std::size_t ID) override;
    std::string serialize() override;

private:
    size_t m_ID;
    std::string m_objectType;
    std::string m_textContent;
    Point m_topLeftCorner;
    size_t m_width;
};

class Polygon : public ObjectWithAttributes {
public:
    size_t getID() override;
    void setID(std::size_t ID) override;
    std::string serialize() override;

private:
    size_t m_ID;
    std::string m_objectType;
    size_t m_numberOfVertex;
    std::vector<Point> m_arrayOfVertexCoordinates;
};

class Ellipse : public ObjectWithAttributes {
public:
    size_t getID() override;
    void setID(std::size_t ID) override;
    std::string serialize() override;

private:
    size_t m_ID;
    std::string m_objectType;
    Point m_center;
    size_t m_xRadius;
    size_t m_yRadius;
};