#include "LineItem.h"

#include "DrawSceneModel.h"

namespace inklink::draw
{
LineItem::LineItem(DrawSceneModel* model, QGraphicsItem* parent)
        : ObjectWithAttributes(parent), m_model{model}, m_selected(false), m_selectedPoint(-1)
{
    setFlag(ItemIsSelectable);
    setAcceptHoverEvents(true);
    setZValue(1); // Ensure the line is drawn on top of other items
}

QRectF LineItem::boundingRect() const
{
    return {m_line.p1(), m_line.p2()};
}

void LineItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::Antialiasing);

    if (option->state & QStyle::State_Selected)
        painter->setPen(Qt::red);
    else
        painter->setPen(Qt::black);

    painter->drawLine(m_line);
    drawPoints(painter);
}

void LineItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        const QPointF pos = event->pos();
        const qreal pointSize = m_pointSize / 2;
        const QRectF startRect(m_line.p1() - QPointF(pointSize, pointSize), QSizeF(m_pointSize, m_pointSize));
        const QRectF endRect(m_line.p2() - QPointF(pointSize, pointSize), QSizeF(m_pointSize, m_pointSize));

        if (startRect.contains(pos))
        {
            m_selected = true;
            m_selectedPoint = 0;
            return;
        }

        if (endRect.contains(pos))
        {
            m_selected = true;
            m_selectedPoint = 1;
            return;
        }

        m_selected = true;
        m_selectedPoint = -1;
    }
}

void LineItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (m_selected)
    {
        if (m_selectedPoint == 0)
        {
            m_line.setP1(event->pos());
        }
        else if (m_selectedPoint == 1)
        {
            m_line.setP2(event->pos());
        }
        else
        {
            const QPointF delta = event->pos() - event->lastPos();
            m_line.translate(delta);
        }

        prepareGeometryChange();
        update();
    }
}

void LineItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_selected = false;
        m_selectedPoint = -1;

        DataContainer polygonInfo;

        polygonInfo["number of angles"] = 2;

        std::vector<DataContainer> anglesArray;

        QPointF startPoint = m_line.p1();
        QPointF endPoint = m_line.p2();

        DataContainer startPointContainer;
        startPointContainer["x"] = startPoint.x();
        startPointContainer["y"] = startPoint.y();

        DataContainer endPointContainer;
        endPointContainer["x"] = endPoint.x();
        endPointContainer["y"] = endPoint.y();

        anglesArray.push_back(startPointContainer);
        anglesArray.push_back(endPointContainer);

        polygonInfo["angles coordinates"] = anglesArray;

        //            std::cout << polygonInfo << std::endl;
    }
}

QPainterPath LineItem::shape() const
{
    QPainterPath path;
    path.moveTo(m_line.p1());
    path.lineTo(m_line.p2());
    return path;
}

void LineItem::drawPoints(QPainter* painter) const
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::black);

    const qreal pointSize = m_pointSize / 2;
    const QRectF startRect(m_line.p1() - QPointF(pointSize, pointSize), QSizeF(m_pointSize, m_pointSize));
    const QRectF endRect(m_line.p2() - QPointF(pointSize, pointSize), QSizeF(m_pointSize, m_pointSize));

    painter->drawEllipse(startRect);
    painter->drawEllipse(endRect);
}
} // namespace inklink::draw
