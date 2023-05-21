#pragma once

#include <QGraphicsItem>

class VertexView;

namespace inklink::graph
{
class EdgeView : public QGraphicsItem
{
public:
    EdgeView();

    [[nodiscard]] VertexView* SourceVertex() const;
    [[nodiscard]] VertexView* DestinationVertex() const;
    void Adjust();

protected:
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

private:
    VertexView* source;
    VertexView* destination;
    QPointF sourcePoint;
    QPointF destPoint;
};
} // namespace inklink::graph