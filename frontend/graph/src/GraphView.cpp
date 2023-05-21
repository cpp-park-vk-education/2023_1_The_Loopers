#include "GraphView.h"

#include <QGraphicsView>

namespace inklink::graph
{
GraphView::GraphView(QWidget* parent) : QGraphicsView(parent)
{
    QGraphicsScene* scene = new QGraphicsScene(this);
    scene->setSceneRect(-640, -460, 1280, 920);
    setScene(scene);
    setFrameShape(QFrame::NoFrame);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void GraphView::wheelEvent(QWheelEvent* event)
{
    ScaleView(pow(2., event->angleDelta().y() / 400.0));
}

void GraphView::ScaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.5 || factor > 5)
        return;

    scale(scaleFactor, scaleFactor);
}

void GraphView::drawBackground(QPainter* painter, const QRectF& rect)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(30, 30, 30));
    painter->drawRect(rect);
}
} // namespace inklink::graph
