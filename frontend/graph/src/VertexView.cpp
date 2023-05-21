#include "VertexView.h"

#include <QPainter>
#include <QRadialGradient>
#include <QStyle>

namespace inklink::graph
{
VertexView::VertexView(GraphView* graphView) : m_graph(graphView)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(1);
}

void VertexView::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*)
{
    QRadialGradient gradient(0, 0, 10);
    if (option->state & QStyle::State_Sunken)
    {
        gradient.setColorAt(1, QColor(124, 90, 237).darker(100));
        gradient.setColorAt(0, QColor(124, 90, 237).lighter(100));
    }
    else
    {
        gradient.setColorAt(0, QColor(218, 218, 218));
        gradient.setColorAt(1, QColor(218, 218, 218));
    }

    painter->setBrush(gradient);
    painter->setPen(QPen(gradient, 0));
    painter->drawEllipse(-10, -10, 20, 20);
}

QRectF VertexView::boundingRect() const
{
    qreal penWidth = 2;
    return QRectF(-10 - penWidth / 2, -10 - penWidth / 2, 23 + penWidth / 2, 23 + penWidth / 2);
}

QPainterPath VertexView::shape() const
{
    QPainterPath path;
    path.addEllipse(-10, -10, 20, 20);
    return path;
}
} // namespace inklink::graph