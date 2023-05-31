#pragma once

#include "IObject.h"

#include <data_container.h>

#include <QGraphicsObject>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

namespace inklink::draw
{
class EllipseItem : public ObjectWithAttributes
{
public:
    EllipseItem(QGraphicsItem *parent = nullptr);

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
    int m_size;
    bool m_selected;
};
} // namespace inklink::draw
