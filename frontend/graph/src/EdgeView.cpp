#include "EdgeView.h"
#include "VertexView.h"

#include <QPainter>
#include <QtMath>
#include <QStyleOption>

namespace inklink::graph
{
EdgeView::EdgeView(VertexView* source, VertexView* destination) : m_source(source), m_destination(destination)
{
    source->AddEdge(this);
    destination->AddEdge(this);
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

void EdgeView::Adjust()
{
    if (!m_source || !m_destination)
        return;

    QLineF line(mapFromItem(m_source, 0, 0), mapFromItem(m_destination, 0, 0));
    qreal length = line.length();

    prepareGeometryChange();

    if (length > qreal(20.)) {
        QPointF edgeOffset((line.dx() * 10) / length, (line.dy() * 10) / length);
        m_sourcePoint = line.p1() + edgeOffset;
        m_destinationPoint = line.p2() - edgeOffset;
    } else {
        m_sourcePoint = m_destinationPoint = line.p1();
    }
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
