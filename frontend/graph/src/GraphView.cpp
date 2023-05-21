#include "GraphView.h"

namespace inklink::graph
{
void GraphView::wheelEvent(QWheelEvent *event)
{
    scaleView(pow(2., event->angleDelta().y() / 400.0));
}

void GraphView::scaleView(qreal scaleFactor) {
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.5 || factor > 5)
        return;

    scale(scaleFactor, scaleFactor);
}

void GraphView::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(30, 30, 30));
    painter->drawRect(rect);
}
} // namespace inklink::graph