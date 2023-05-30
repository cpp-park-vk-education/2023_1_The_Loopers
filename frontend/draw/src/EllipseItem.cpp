#include "EllipseItem.h"

EllipseItem::EllipseItem(QGraphicsItem *parent = nullptr) : QGraphicsObject(parent), m_size(100), m_selected(false)
{
    setFlag(ItemIsSelectable);
    setAcceptHoverEvents(true);
}

QRectF EllipseItem::boundingRect() const
{
    qreal adjust = 1.0;
    return QRectF(-m_size / 2 - adjust, -m_size / 2 - adjust, m_size + adjust, m_size + adjust);
}

void EllipseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::Antialiasing);

    if (option->state & QStyle::State_Selected)
        painter->setPen(Qt::red);
    else
        painter->setPen(Qt::black);

    painter->drawEllipse(-m_size / 2, -m_size / 2, m_size, m_size);
}

void EllipseItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        m_selected = true;
}

void EllipseItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_selected)
    {
        setPos(mapToParent(event->pos()));
        m_size += event->delta().y();
        prepareGeometryChange();
        update();
    }
}

void EllipseItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        m_selected = false;

    DataContainer ellipseInfo;

    QPointF center = mapToParent(event->pos());
    DataContainer centerContainer;
    centerContainer["x"] = center.x();
    centerContainer["y"] = center.y();
    ellipseInfo["center"] = centerContainer;

    int xRadius = m_size / 2;
    int yRadius = m_size / 2;
    ellipseInfo["x_radius"] = xRadius;
    ellipseInfo["y_radius"] = yRadius;
}
