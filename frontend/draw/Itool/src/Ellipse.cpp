#include "Ellipse.hpp"

#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <vector>

EllipseDrawingWidget::EllipseDrawingWidget(QWidget *parent) : QWidget(parent) {
    setMouseTracking(true); // Включаем отслеживание движения мыши
}

void EllipseDrawingWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); // Сглаживание

    for (const auto& ellipse : ellipses) // Рисуем все сохранённые эллипсы
    {
        painter.drawEllipse(ellipse);
    }
}

void EllipseDrawingWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton)
    {
        startPoint = event->pos();
        endPoint = startPoint;

        // Создаем эллипс с начальными координатами (точка - прямоугольник 1x1)
        QRect ellipse(startPoint, QSize(1, 1));
        ellipses.push_back(ellipse);
    }
}

void EllipseDrawingWidget::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton)
    {
        endPoint = event->pos();

        // Обновляем размеры последнего эллипса в коллекции
        QRect& lastEllipse = ellipses.back();
        lastEllipse.setBottomRight(endPoint);

        update(); // Перерисовываем холст
    }
}
