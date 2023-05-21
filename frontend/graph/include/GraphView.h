#pragma once

#include <QGraphicsView>
#include <QKeyEvent>

namespace inklink::graph
{
class GraphView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphView(QWidget *parent = nullptr);

protected:
    void wheelEvent(QWheelEvent *event) override;
    void ScaleView(qreal scaleFactor);
    void drawBackground(QPainter *painter, const QRectF &rect) override;
};
} // namespace inklink::graph
