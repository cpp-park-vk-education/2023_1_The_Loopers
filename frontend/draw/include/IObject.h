#pragma once

#include <data_container.h>

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
    using DataContainer = serializer::DataContainer;

public:
    virtual ~ObjectWithAttributes() = default;

    virtual int getID()
    {
        return m_ID;
    }
    virtual void setID(int ID)
    {
        m_ID = ID;
    }
    virtual std::string serialize() = 0;
    virtual void parse(const DataContainer&) = 0;

    // private:
    int m_ID;
};

class TextBox : public ObjectWithAttributes
{
public:
    std::string serialize() override;
    void parse(const DataContainer&) override;

    // private:
    std::string m_objectType;
    std::string m_textContent;
    Point m_topLeftCorner;
    int m_width;
};

class Polygon : public ObjectWithAttributes
{
public:
    std::string serialize() override;
    void parse(const DataContainer&) override;

    // private:
    std::string m_objectType;
    int m_numberOfVertex;
    std::vector<Point> m_arrayOfVertexCoordinates;
};

class Ellipse : public ObjectWithAttributes
{
public:
    std::string serialize() override;
    void parse(const DataContainer&) override;

    // private:
    std::string m_objectType;
    Point m_center;
    int m_xRadius;
    int m_yRadius;
};
} // namespace inklink::draw
