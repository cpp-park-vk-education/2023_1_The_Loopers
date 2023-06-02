#include "RectangleItem.h"

#include "DrawSceneModel.h"

#include <json_serializer.h>

namespace
{
constexpr qreal kTolerance = 10;

using JsonSerializer = inklink::serializer::JsonSerializer;
using DataContainer = inklink::serializer::DataContainer;
} // namespace

namespace inklink::draw
{
RectangleItem::RectangleItem(DrawSceneModel* model, QGraphicsItem* parent)
        : ObjectWithAttributes(parent), m_model{model}, m_rect(QRectF(0, 0, 100, 100)), m_pressedVertex(VertexState::kNone)
{
    setFlag(ItemIsSelectable);
    setAcceptHoverEvents(true);
}

QRectF RectangleItem::boundingRect() const
{
    return m_rect;
}

void RectangleItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(widget);

    painter->setPen(QPen(Qt::black, 2));
    painter->setBrush(option->state & QStyle::State_Sunken ? Qt::red : Qt::blue);
    painter->drawRect(m_rect);

    update();
}

void RectangleItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_pressedVertex = isVertexPressed(event->pos());
        m_lastPos = event->scenePos();
        event->accept();
    }
    else
    {
        event->ignore();
    }

    update();
}

void RectangleItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        if (m_pressedVertex != VertexState::kNone)
        {
            qreal dx = event->scenePos().x() - m_lastPos.x();
            qreal dy = event->scenePos().y() - m_lastPos.y();

            switch (m_pressedVertex) {
            case VertexState::kTopLeft:
                m_rect.setTopLeft(m_rect.topLeft() + QPointF(dx, dy));
                break;
            case VertexState::kTopRight:
                m_rect.setTopRight(m_rect.topRight() + QPointF(dx, dy));
                break;
            case VertexState::kBottomRight:
                m_rect.setBottomRight(m_rect.bottomRight() + QPointF(dx, dy));
                break;
            case VertexState::kBottomLeft:
                m_rect.setBottomLeft(m_rect.bottomLeft() + QPointF(dx, dy));
                break;
            default:
                break;
            }

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

    update();
}

void RectangleItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        DataContainer rectangleData{};

        rectangleData["number of angles"] = 4;

        std::vector<DataContainer> angleCoordinates;

        angleCoordinates.push_back(createPointData(m_rect.topLeft()));
        angleCoordinates.push_back(createPointData(m_rect.topRight()));
        angleCoordinates.push_back(createPointData(m_rect.bottomRight()));
        angleCoordinates.push_back(createPointData(m_rect.bottomLeft()));

        rectangleData["angles coordinates"] = angleCoordinates;

        // std::cout << "RectangleInfo: " << rectangleData.serialize() << std::endl;

        auto msg = JsonSerializer::SerializeAsString(rectangleData);

        m_pressedVertex = VertexState::kNone;
        event->accept();
    }
    else
    {
        event->ignore();
    }
    QGraphicsObject::mouseReleaseEvent(event);
}

RectangleItem::VertexState RectangleItem::isVertexPressed(const QPointF& pos) const
{
    if (qAbs(pos.x() - m_rect.left()) <= kTolerance && qAbs(pos.y() - m_rect.top()) <= kTolerance)
    {
        return VertexState::kTopLeft;
    }

    if (qAbs(pos.x() - m_rect.right()) <= kTolerance && qAbs(pos.y() - m_rect.top()) <= kTolerance)
    {
        return VertexState::kTopRight;
    }

    if (qAbs(pos.x() - m_rect.right()) <= kTolerance && qAbs(pos.y() - m_rect.bottom()) <= kTolerance)
    {
        return VertexState::kBottomRight;
    }

    if (qAbs(pos.x() - m_rect.left()) <= kTolerance && qAbs(pos.y() - m_rect.bottom()) <= kTolerance)
    {
        return VertexState::kBottomLeft;
    }

    return VertexState::kNone;
}

DataContainer RectangleItem::createPointData(const QPointF& point)
{
    DataContainer pointData;
    pointData["xPosition"] = point.x();
    pointData["yPosition"] = point.y();
    return pointData;
}
} // namespace inklink::draw
