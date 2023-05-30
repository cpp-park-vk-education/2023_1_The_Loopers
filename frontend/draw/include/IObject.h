#pragma once

#include <string>
#include <vector>

namespace inklink::draw
{
struct Point
{
    int xPosition;
    int yPosition;
};

class ObjectWithAttributes
{
public:
    virtual ~ObjectWithAttributes() = default;

    virtual int getID() = 0;
    virtual void setID(int ID) = 0;
    virtual std::string serialize() = 0;

//private:
    int m_ID;
};

class TextBox : public ObjectWithAttributes
{
public:
    int getID() override;
    void setID(int ID) override;
    std::string serialize() override;

//private:
    int m_ID;
    std::string m_objectType;
    std::string m_textContent;
    Point m_topLeftCorner;
    int m_width;
};

class Polygon : public ObjectWithAttributes
{
public:
    int getID() override;
    void setID(int ID) override;
    std::string serialize() override;

//private:
    int m_ID;
    std::string m_objectType;
    int m_numberOfVertex;
    std::vector<Point> m_arrayOfVertexCoordinates;
};

class Ellipse : public ObjectWithAttributes
{
public:
    int getID() override;
    void setID(int ID) override;
    std::string serialize() override;

//private:
    int m_ID;
    std::string m_objectType;
    Point m_center;
    int m_xRadius;
    int m_yRadius;
};
} // namespace inklink::draw
