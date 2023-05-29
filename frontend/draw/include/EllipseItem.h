#include <QGraphicsObject>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QMouseEvent>

class EllipseItem : public QGraphicsObject
{
public:
    EllipseItem(QGraphicsItem *parent = nullptr)
            : QGraphicsObject(parent), m_size(100), m_selected(false)
    {
        setFlag(ItemIsSelectable);
        setAcceptHoverEvents(true);
    }

    QRectF boundingRect() const override
    {
        qreal adjust = 1.0;
        return QRectF(-m_size / 2 - adjust, -m_size / 2 - adjust, m_size + adjust, m_size + adjust);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override
    {
        Q_UNUSED(widget);

        painter->setRenderHint(QPainter::Antialiasing);

        if (option->state & QStyle::State_Selected)
            painter->setPen(Qt::red);
        else
            painter->setPen(Qt::black);

        painter->drawEllipse(-m_size / 2, -m_size / 2, m_size, m_size);
    }

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton)
            m_selected = true;
    }

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override
    {
        if (m_selected)
        {
            setPos(mapToParent(event->pos()));
            m_size += event->delta().y();
            prepareGeometryChange();
            update();
        }
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton)
            m_selected = false;
    }

private:
    int m_size;
    bool m_selected;
};
