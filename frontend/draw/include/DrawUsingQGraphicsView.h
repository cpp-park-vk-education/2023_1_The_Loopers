#pragma once

#pragma once

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QMouseEvent>

class DrawingView : public QGraphicsView
{
public:
    DrawingView(QWidget* parent = nullptr)
            : QGraphicsView(parent), scene(new QGraphicsScene(this)), drawing(false)
    {
        setScene(scene);
    }

protected:
    void mousePressEvent(QMouseEvent* event) override
    {
        if (event->button() == Qt::LeftButton)
        {
            drawing = true;
            startPoint = mapToScene(event->pos());
            endPoint = startPoint;
        }
        QGraphicsView::mousePressEvent(event);
    }

    void mouseReleaseEvent(QMouseEvent* event) override
    {
        if (event->button() == Qt::LeftButton && drawing)
        {
            endPoint = mapToScene(event->pos());
            QGraphicsLineItem* line = new QGraphicsLineItem(QLineF(startPoint, endPoint));
            line->setPen(QPen(Qt::red));
            scene->addItem(line);
            drawing = false;
        }
        QGraphicsView::mousePressEvent(event);
    }

    void mouseMoveEvent(QMouseEvent* event) override
    {
        if (drawing)
        {
            startPoint = endPoint;
            endPoint = mapToScene(event->pos());
            QGraphicsLineItem* tempLine = new QGraphicsLineItem(QLineF(startPoint, endPoint));
            tempLine->setPen(QPen(Qt::red));
            scene->addItem(tempLine);
        }
        QGraphicsView::mousePressEvent(event);
    }

private:
    QGraphicsScene* scene;
    bool drawing;
    QPointF startPoint;
    QPointF endPoint;
    QPointF lastPos;

    QGraphicsItem* movingItem;
    QPointF movingOffset;
};
