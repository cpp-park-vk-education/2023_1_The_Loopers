#pragma once

#include <data_container.h>

#include <QGraphicsItem>

#include <chrono>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

namespace inklink::draw
{
struct Point
{
    int xPosition;
    int yPosition;
};

class ObjectWithAttributes : public QGraphicsItem
{
public:
    using DataContainer = serializer::DataContainer;

public:
    ObjectWithAttributes() : QGraphicsItem()
    {
        GenerateID();
    }

    // everything else deleted for now

    virtual ~ObjectWithAttributes() = default;

    virtual std::string getID()
    {
        return m_ID;
    }

    virtual std::string serialize() = 0;
    virtual void parse(const DataContainer &) = 0;

signals:
    void Changed(const char *);

protected:
    // did not send it yet: because still in progress
    // All changes will be improved on server, because blocking
    std::unordered_map<std::string /*action id*/, DataContainer /*changes*/> m_notSent;

    std::string m_ID{""};

private:
    virtual void GenerateID()
    {
        if (m_ID.empty())
        {
            return;
        }
        static unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        static std::mt19937_64 gen{seed};

        std::stringstream ss;
        for (size_t i = 0; i < 2; ++i)
        {
            ss << gen();
        }
        m_ID = ss.str();
        m_ID = ID;
    }
};

class TextBox : public ObjectWithAttributes
{
public:
    std::string serialize() override;
    void parse(const DataContainer &) override;

private:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override; // signal changed: selected then wait until answer
    void
    mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override; // signal changed: pass all changes for this "session"

private:
    std::string m_objectType;
    std::string m_textContent;
    Point m_topLeftCorner;
    int m_width;
};

class Polygon : public ObjectWithAttributes
{
public:
    std::string serialize() override;
    void parse(const DataContainer &) override;

private:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override; // signal changed: selected then wait until answer
    void
    mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override; // signal changed: pass all changes for this "session"

private:
    std::string m_objectType;
    int m_numberOfVertex;
    std::vector<Point> m_arrayOfVertexCoordinates;
};

class Ellipse : public ObjectWithAttributes
{
public:
    std::string serialize() override;
    void parse(const DataContainer &) override;

private:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override; // signal changed: selected then wait until answer
    void
    mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override; // signal changed: pass all changes for this "session"

private:
    std::string m_objectType;
    Point m_center;
    int m_xRadius;
    int m_yRadius;
};
} // namespace inklink::draw
