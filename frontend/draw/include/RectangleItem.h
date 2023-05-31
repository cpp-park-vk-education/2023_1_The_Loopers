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

    bool isVertexPressed(const QPointF& pos) const;

private:
    DataContainer createPointData(const QPointF& point);

    QRectF m_rect;
    QPointF m_lastPos;
    bool m_isResizing = false;

    DrawSceneModel* m_model;
};
} // namespace inklink::draw
