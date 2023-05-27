#include "Rectangle.hpp"

RectangleDrawingWidget::RectangleDrawingWidget(QWidget* parent) : QWidget(parent)
{
    setMouseTracking(true); // Включаем отслеживание движения мыши
}

// void RectangleDrawingWidget::paintEvent(QPaintEvent* event) {
//     QPainter painter(this);
//     painter.setRenderHint(QPainter::Antialiasing); // Сглаживание
//
//     // Рисуем все сохраненные элементы
//     for (const auto &item: items) {
//         if (item.type() == QVariant::Rect) {
//             QRect rect = item.value<QRect>();
//             painter.drawRect(rect);
//         }
//     }
// }

void RectangleDrawingWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        startPoint = event->pos();
        endPoint = startPoint;

        // Создаем элемент типа QRect с начальными координатами (ширина и высота равны 0)
        QRect rect(startPoint, startPoint);
        items.push_back(QVariant::fromValue(rect));

        update(); // Перерисовываем холст
    }
}

void RectangleDrawingWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        endPoint = event->pos();

        // Обновляем размеры последнего элемента в коллекции
        if (!items.empty())
        {
            QVariant &lastItem = items.back();
            if (lastItem.type() == QVariant::Rect)
            {
                QRect rect = lastItem.value<QRect>();
                rect.setBottomRight(endPoint);
                lastItem = QVariant::fromValue(rect);
            }
        }

        update(); // Перерисовываем холст
    }
}
