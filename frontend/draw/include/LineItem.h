#pragma once

#include "IObject.h"

#include <DataContainer.h>

#include <QGraphicsObject>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

class LineItem : public ObjectWithAttributes
{
public:
    LineItem(QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

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
