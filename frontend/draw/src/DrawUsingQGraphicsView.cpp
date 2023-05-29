#include "DrawUsingQGraphicsView.h"

#include "DrawSceneModel.h"

#include <QApplication>
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>

#include <iostream>

namespace inklink::draw
{

DrawingView::DrawingView(QWidget* parent)
        : m_scene{new DrawSceneModel(this)}, QGraphicsView(m_scene, parent), drawing(false)
{
}

void DrawingView::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        drawing = true;
        startPoint = mapToScene(event->pos());
        endPoint = startPoint;
    }
    QGraphicsView::mousePressEvent(event);
}

void DrawingView::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && drawing)
    {
        endPoint = mapToScene(event->pos());
        QGraphicsLineItem* line = new QGraphicsLineItem(QLineF(startPoint, endPoint));
        line->setPen(QPen(Qt::red));
        m_scene->addItem(line);
        drawing = false;
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void DrawingView::mouseMoveEvent(QMouseEvent* event)
{
    if (drawing)
    {
        startPoint = endPoint;
        endPoint = mapToScene(event->pos());
        QGraphicsLineItem* tempLine = new QGraphicsLineItem(QLineF(startPoint, endPoint));
        tempLine->setPen(QPen(Qt::red));
        m_scene->addItem(tempLine);
    }
    QGraphicsView::mouseMoveEvent(event);
}
} // namespace inklink::draw
