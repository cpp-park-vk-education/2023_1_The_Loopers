#include "GraphView.h"

namespace inklink::graph
{
void GraphView::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(30, 30, 30));
    painter->drawRect(rect);
}
} // namespace inklink::graph