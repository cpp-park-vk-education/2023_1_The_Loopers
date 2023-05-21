#pragma once

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOption>

class GraphView;

namespace inklink::graph
{
class VertexView : public QGraphicsItem
{
public:
    VertexView(GraphView* graphView);

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    QRectF boundingRect() const override;
    QPainterPath shape() const override;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

private:
    GraphView* m_graph;
};
}