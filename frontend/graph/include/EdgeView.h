#pragma once

#include <QGraphicsItem>

class VertexView;

class EdgeView : public QGraphicsItem
{
public:
    EdgeView(VertexView* sourceVertex, VertexView* destinationVertex);

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