#pragma once

#include "IObject.h"

#include <data_container.h>

#include <QGraphicsObject>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

namespace inklink::draw
{
class LineItem : public ObjectWithAttributes
{
public:
    LineItem(QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    std::string serialize() override
    {
    }
    void parse(const DataContainer &) override
    {
    }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QPainterPath shape() const override;

    void drawPoints(QPainter *painter) const;

    QLineF m_line;
    bool m_selected;
    int m_selectedPoint;
    const qreal m_pointSize = 8.0;
};
} // namespace inklink::draw
