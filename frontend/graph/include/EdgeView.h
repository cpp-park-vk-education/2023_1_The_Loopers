#pragma once

#include <QGraphicsItem>

namespace inklink::graph
{
class VertexView;

class EdgeView : public QGraphicsItem
{
public:
    EdgeView(VertexView* source, VertexView* destination);

    [[nodiscard]] VertexView* SourceVertex() const;
    [[nodiscard]] VertexView* DestinationVertex() const;
    void Adjust();

protected:
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

private:
    VertexView* m_source;
    VertexView* m_destination;
    QPointF m_sourcePoint;
    QPointF m_destinationPoint;
};
} // namespace inklink::graph
