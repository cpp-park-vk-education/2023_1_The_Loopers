#include "DrawUsingQGraphicsView.h"

#include "DrawSceneModel.h"

#include <QApplication>
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>

namespace{
constexpr QColor k_currentColor(255,255,255);
}

namespace inklink::draw
{

DrawingView::DrawingView(QWidget* parent) : QGraphicsView(parent), drawing(false)
{
    m_scene = new DrawSceneModel(this);
    setScene(m_scene);
    setGeometry(0, 0, 600, 600);
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
        line->setPen(QPen(k_currentColor));
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
        tempLine->setPen(QPen(k_currentColor));
        m_scene->addItem(tempLine);
    }
    QGraphicsView::mouseMoveEvent(event);
}
} // namespace inklink::draw
