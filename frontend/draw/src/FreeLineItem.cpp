#include "FreeLineItem.h"

FreeLineItem::FreeLineItem(QGraphicsItem *parent) : ObjectWithAttributes(parent), m_selected(false)
{
    setFlag(ItemIsSelectable);
    setAcceptHoverEvents(true);
}

QRectF FreeLineItem::boundingRect() const override
{
    qreal maxX = -std::numeric_limits<qreal>::max();
    qreal maxY = -std::numeric_limits<qreal>::max();
    qreal minX = std::numeric_limits<qreal>::max();
    qreal minY = std::numeric_limits<qreal>::max();

    for (const QPointF &point : m_points)
    {
        maxX = std::max(maxX, point.x());
        maxY = std::max(maxY, point.y());
        minX = std::min(minX, point.x());
        minY = std::min(minY, point.y());
    }

    qreal adjust = 1.0;
    return QRectF(minX - adjust, minY - adjust, maxX - minX + 2 * adjust, maxY - minY + 2 * adjust);
}

void FreeLineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override
{
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::Antialiasing);

    if (option->state & QStyle::State_Selected)
        painter->setPen(Qt::red);
    else
        painter->setPen(Qt::black);

    painter->drawPolyline(m_points.data(), m_points.size());
}

void FreeLineItem::mousePressEvent(QGraphicsSceneMouseEvent *event) override
{
    if (event->button() == Qt::LeftButton)
    {
        m_selected = false;

        DataContainer polygonInfo;

        polygonInfo["number of angles"] = m_points.size();

        std::vector<DataContainer> anglesArray;

        for (const QPointF &point : m_points)
        {
            DataContainer pointContainer;

            pointContainer["x"] = point.x();
            pointContainer["y"] = point.y();

            anglesArray.push_back(pointContainer);
        }

        polygonInfo["angles coordinates"] = anglesArray;

        //            std::cout << polygonInfo << std::endl;
    }
}

void FreeLineItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) override
{
    if (m_selected)
    {
        m_points << mapToParent(event->pos());
        prepareGeometryChange();
        update();
    }
}

void FreeLineItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override
{
    if (event->button() == Qt::LeftButton)
        m_selected = false;
}
