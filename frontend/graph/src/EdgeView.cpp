#include "EdgeView.h"

#include <QPainter>

namespace inklink::graph
{
EdgeView::EdgeView(VertexView* source, VertexView* destination) : m_source(source), m_destination(destination)
{
    Adjust();
}

VertexView* EdgeView::SourceVertex() const
{
    return m_source;
}

VertexView* EdgeView::DestinationVertex() const
{
    return m_destination;
}

QRectF EdgeView::boundingRect() const
{
    if (!m_source || !m_destination)
        return QRectF();

    qreal penWidth = 10;
    return QRectF(m_sourcePoint,
                  QSizeF(m_destinationPoint.x() - m_sourcePoint.x(), m_destinationPoint.y() - m_sourcePoint.y()))
            .normalized()
            .adjusted(-penWidth/2, -penWidth/2, penWidth/2, penWidth/2);
}

void EdgeView::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*)
{
    if (!m_source || !m_destination)
        return;

    QLineF line(m_sourcePoint, m_destinationPoint);
    painter->setPen(QPen(QColor(102, 102, 102), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(line);
}
} // namespace inklink::graph