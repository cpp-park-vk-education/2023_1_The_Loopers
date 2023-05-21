#include "VertexView.h"

namespace inklink::graph
{
VertexView::VertexView(GraphView *graphView) : m_graph(graphView)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(1);
}

QRectF VertexView::boundingRect() const
{
    qreal penWidth = 2;
    return QRectF( -10 - penWidth/2, -10 - penWidth/2, 23 + penWidth/2, 23 + penWidth/2);
}

QPainterPath VertexView::shape() const
{
    QPainterPath path;
    path.addEllipse(-10, -10, 20, 20);
    return path;
}
} // namespace inklink::graph