#include "RectangleItem.h"

RectangleItem::RectangleItem(QGraphicsItem* parent) : ObjectWithAttributes(parent), m_rect(QRectF(0, 0, 100, 100))
{
    setFlag(ItemIsSelectable);
    setAcceptHoverEvents(true);
}

QRectF RectangleItem::boundingRect() const override
{
    return m_rect;
}

void RectangleItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setPen(QPen(Qt::black, 2));
    painter->setBrush(isSelected() ? Qt::red : Qt::blue);
    painter->drawRect(m_rect);
}

void RectangleItem::mousePressEvent(QGraphicsSceneMouseEvent* event) override
{
    if (event->button() == Qt::LeftButton)
    {
        m_isResizing = isVertexPressed(event->pos());
        m_lastPos = event->scenePos();
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void RectangleItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event) override
{
    if (event->buttons() & Qt::LeftButton)
    {
        if (m_isResizing)
        {
            qreal dx = event->scenePos().x() - m_lastPos.x();
            qreal dy = event->scenePos().y() - m_lastPos.y();
            m_rect.adjust(0, 0, dx, dy);
            prepareGeometryChange();
        }
        else
        {
            qreal dx = event->scenePos().x() - m_lastPos.x();
            qreal dy = event->scenePos().y() - m_lastPos.y();
            m_rect.translate(dx, dy);
            prepareGeometryChange();
        }
        m_lastPos = event->scenePos();
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void RectangleItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override
{
    if (event->button() == Qt::LeftButton)
    {
        DataContainer rectangleData;

        rectangleData["number of angles"] = 4;

        std::vector<DataContainer> angleCoordinates;

        angleCoordinates.push_back(createPointData(topLeft()));
        angleCoordinates.push_back(createPointData(topRight()));
        angleCoordinates.push_back(createPointData(bottomRight()));
        angleCoordinates.push_back(createPointData(bottomLeft()));

        rectangleData["angles coordinates"] = angleCoordinates;

        std::cout << "RectangleInfo: " << rectangleData.serialize() << std::endl;

        m_isResizing = false;
        event->accept();
    }
    else
    {
        event->ignore();
    }
    QGraphicsObject::mouseReleaseEvent(event);
}

bool RectangleItem::isVertexPressed(const QPointF& pos) const
{
    qreal tolerance = 5.0;

    if (qAbs(pos.x() - m_rect.left()) <= tolerance && qAbs(pos.y() - m_rect.top()) <= tolerance)
        return true;

    if (qAbs(pos.x() - m_rect.right()) <= tolerance && qAbs(pos.y() - m_rect.top()) <= tolerance)
        return true;

    if (qAbs(pos.x() - m_rect.left()) <= tolerance && qAbs(pos.y() - m_rect.bottom()) <= tolerance)
        return true;

    if (qAbs(pos.x() - m_rect.right()) <= tolerance && qAbs(pos.y() - m_rect.bottom()) <= tolerance)
        return true;

    return false;
}

DataContainer RectangleItem::createPointData(const QPointF& point)
{
    DataContainer pointData;
    pointData["xPosition"] = point.x();
    pointData["yPosition"] = point.y();
    return pointData;
}
