#include "Line.hpp"

namespace inklink::draw
{
LineWidget::LineWidget(QWidget* parent) : QWidget(parent)
{
    setMouseTracking(true); // Включаем отслеживание движения мыши
}

// void LineWidget::paintEvent(QPaintEvent* event) {
//     QPainter painter(this);
//     painter.setRenderHint(QPainter::Antialiasing); // Сглаживание
//
//     // Рисуем все сохраненные элементы
//     for (const auto &item: m_items) {
//         QLine line = item.value<QLine>();
//         painter.drawLine(line);
//     }
// }

void LineWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_startPoint = event->pos();
        m_endPoint = m_startPoint;

        // Создаем элемент типа QLine с начальными координатами (одна точка - линия длины 0)
        QLine line(m_startPoint, m_startPoint);
        m_items.push_back(QVariant::fromValue(line));

        update(); // Перерисовываем холст
    }
}

void LineWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        m_endPoint = event->pos();

        // Обновляем конечные координаты последнего элемента в коллекции
        if (!m_items.empty())
        {
            QVariant& lastItem = m_items.back();
            if (lastItem.type() == QVariant::Line)
            {
                QLine line = lastItem.value<QLine>();
                line.setP2(m_endPoint);
                lastItem = QVariant::fromValue(line);
            }
        }

        update(); // Перерисовываем холст
    }
}
} // namespace inklink::draw
