#pragma once

#include <QGraphicsView>

class GraphView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphView(QWidget *parent = nullptr);

protected:
    void wheelEvent(QWheelEvent *event) override;
    void scaleView(qreal scaleFactor);
    void drawBackground(QPainter *painter, const QRectF &rect) override;
};