#include "FreeLineItem.h"

#include "DrawSceneModel.h"

namespace inklink::draw
{
FreeLineItem::FreeLineItem(DrawSceneModel* model, QGraphicsItem* parent)
        : ObjectWithAttributes(parent), m_model{model}, m_selected(false)
{
    setFlag(ItemIsSelectable);
    setAcceptHoverEvents(true);
}

QRectF FreeLineItem::boundingRect() const
{
    qreal maxX = -std::numeric_limits<qreal>::max();
    qreal maxY = -std::numeric_limits<qreal>::max();
    qreal minX = std::numeric_limits<qreal>::max();
    qreal minY = std::numeric_limits<qreal>::max();

    for (const QPointF& point : m_points)
    {
        maxX = std::max(maxX, point.x());
        maxY = std::max(maxY, point.y());
        minX = std::min(minX, point.x());
        minY = std::min(minY, point.y());
    }

    qreal adjust = 1.0;
    return QRectF(minX - adjust, minY - adjust, maxX - minX + 2 * adjust, maxY - minY + 2 * adjust);
}

void FreeLineItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::Antialiasing);

    if (option->state & QStyle::State_Selected)
    {
        painter->setPen(Qt::red);
    }
    else
    {
        painter->setPen(Qt::black);
    }

    painter->drawPolyline(m_points.data(), m_points.size());
}

void FreeLineItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_selected = false;

        DataContainer polygonInfo;

        polygonInfo["number of angles"] = static_cast<int>(m_points.size());

        std::vector<DataContainer> anglesArray;

        for (const QPointF& point : m_points)
        {
            DataContainer pointContainer;

            pointContainer["x"] = point.x();
            pointContainer["y"] = point.y();

            anglesArray.push_back(pointContainer);
        }

        polygonInfo["angles coordinates"] = anglesArray;

        //            std::cout << polygonInfo << std::endl;
    }

//    m_points.clear();
    m_points << mapToParent(event->pos());
}

void FreeLineItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (m_selected)
    {
        m_points << mapToParent(event->pos());
        prepareGeometryChange();
        update();
    }
}

void FreeLineItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
        m_selected = false;
}
} // namespace inklink::draw
