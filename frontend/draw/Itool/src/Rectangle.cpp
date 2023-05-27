#include "Rectangle.hpp"

namespace inklink::draw
{
RectangleDrawingWidget::RectangleDrawingWidget(QWidget* parent) : QWidget(parent)
{
    setMouseTracking(true); // Включаем отслеживание движения мыши
}

// void RectangleDrawingWidget::paintEvent(QPaintEvent* event) {
//     QPainter painter(this);
//     painter.setRenderHint(QPainter::Antialiasing); // Сглаживание
//
//     // Рисуем все сохраненные элементы
//     for (const auto &item: m_items) {
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
        m_startPoint = event->pos();
        m_endPoint = m_startPoint;

        // Создаем элемент типа QRect с начальными координатами (ширина и высота равны 0)
        QRect rect(m_startPoint, m_startPoint);
        m_items.push_back(QVariant::fromValue(rect));

        update(); // Перерисовываем холст
    }
}

void RectangleDrawingWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        m_endPoint = event->pos();

        // Обновляем размеры последнего элемента в коллекции
        if (!items.empty())
        {
            QVariant& lastItem = m_items.back();
            if (lastItem.type() == QVariant::Rect)
            {
                QRect rect = lastItem.value<QRect>();
                rect.setBottomRight(m_endPoint);
                lastItem = QVariant::fromValue(rect);
            }
        }

        update(); // Перерисовываем холст
    }
}
} // namespace inklink::draw