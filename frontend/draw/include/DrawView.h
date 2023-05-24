#pragma once

#include <QGraphicsItem>
#include <QMouseEvent>
#include <QWidget>

namespace inklink::draw
{
class DrawView : public QWidget
{
public:
    DrawView(QWidget *parent = nullptr){};

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    bool drawing;
    QGraphicsItem* currentObject;
    QList<QGraphicsItem*> objects;
    QGraphicsItem *selectedObject;
    QPointF lastPoint;
    QPointF lastOffset;
};
} // namespace inklink::draw
