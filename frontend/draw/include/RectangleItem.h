#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsItem>

class RectangleItem : public QGraphicsObject
{
public:
    RectangleItem(QGraphicsItem* parent = nullptr)
            : QGraphicsObject(parent), m_rect(QRectF(0, 0, 100, 100))
    {
        setFlag(ItemIsSelectable);
        setAcceptHoverEvents(true);
    }

    QRectF boundingRect() const override
    {
        return m_rect;
    }

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override
    {
        Q_UNUSED(option);
        Q_UNUSED(widget);

        painter->setPen(QPen(Qt::black, 2));
        painter->setBrush(isSelected() ? Qt::red : Qt::blue);
        painter->drawRect(m_rect);
    }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override
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

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override
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

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override
    {
        if (event->button() == Qt::LeftButton)
        {
            m_isResizing = false;
            event->accept();
        }
        else
        {
            event->ignore();
        }
    }

    bool isVertexPressed(const QPointF& pos) const
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

private:
    QRectF m_rect;
    QPointF m_lastPos;
    bool m_isResizing = false;
};
