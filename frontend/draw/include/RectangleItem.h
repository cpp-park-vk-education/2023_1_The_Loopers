#pragma once

#include "IObject.h"

#include <data_container.h>

#include <QGraphicsObject>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

namespace inklink::draw
{
class DrawSceneModel;

class RectangleItem : public ObjectWithAttributes
{
public:
    RectangleItem(DrawSceneModel*, QGraphicsItem* parent = nullptr);

    QRectF boundingRect() const override;

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    std::string serialize() override
    {
        return "";
    }
    void parse(const DataContainer&) override
    {
    }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

private:
    enum class VertexState
    {
        kTopLeft,
        kTopRight,
        kBottomRight,
        kBottomLeft,
        kNone
    };

    VertexState isVertexPressed(const QPointF& pos) const;
    DataContainer createPointData(const QPointF& point);

    DrawSceneModel* m_model;
    QRectF m_rect;
    QPointF m_lastPos;
    VertexState m_pressedVertex;
};
} // namespace inklink::draw
