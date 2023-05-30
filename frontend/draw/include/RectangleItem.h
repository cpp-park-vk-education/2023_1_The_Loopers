#include "IObject.h"

#include <DataContainer.h>

#include <QGraphicsObject>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

class RectangleItem : public ObjectWithAttributes
{
public:
    RectangleItem(QGraphicsItem* parent = nullptr);

    QRectF boundingRect() const override;

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

    bool isVertexPressed(const QPointF& pos) const;

private:
    DataContainer createPointData(const QPointF& point);

    QRectF m_rect;
    QPointF m_lastPos;
    bool m_isResizing = false;
};
