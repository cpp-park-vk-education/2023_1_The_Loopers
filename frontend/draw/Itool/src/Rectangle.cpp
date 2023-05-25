#include "Rectangle.hpp"

#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QWidget>

RectangleDrawingWidget::RectangleDrawingWidget(QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true); // Включаем отслеживание движения мыши
}

void RectangleDrawingWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); // Сглаживание

    // Рисуем все сохраненные прямоугольники
    for (const auto &rectangle : rectangles)
    {
        painter.drawRect(rectangle);
    }
}

void RectangleDrawingWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        startPoint = event->pos();
        endPoint = startPoint;

        // Создаем прямоугольник с начальными координатами (точка - прямоугольник 1x1)
        QRect rectangle(startPoint, QSize(1, 1));
        rectangles.push_back(rectangle);
    }
}

void RectangleDrawingWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        endPoint = event->pos();

        // Обновляем размеры последнего прямоугольника в коллекции
        QRect &lastRectangle = rectangles.back();
        lastRectangle.setBottomRight(endPoint);

        update(); // Перерисовываем холст
    }
}
