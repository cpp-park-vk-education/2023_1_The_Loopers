#pragma once

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOption>

namespace inklink::graph
{
class GraphView;
class EdgeView;

class VertexView : public QGraphicsItem
{
public:
    VertexView(GraphView* graphView);

    void AddEdge(EdgeView *edge);
    QList<EdgeView *> GetEdges() const;

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    QRectF boundingRect() const override;
    QPainterPath shape() const override;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

private:
    QList<EdgeView *> m_edgeList;
    QPointF m_newPosition;
    GraphView* m_graph;
};
}
