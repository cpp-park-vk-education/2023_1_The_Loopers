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

class EllipseItem : public ObjectWithAttributes
{
public:
    EllipseItem(DrawSceneModel*, QGraphicsItem* parent = nullptr);

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
    int m_size;
    bool m_selected;

    DrawSceneModel* m_model;
};
} // namespace inklink::draw
