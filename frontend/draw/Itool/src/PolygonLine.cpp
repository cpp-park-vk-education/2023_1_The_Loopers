#include "PolygonLine.hpp"

namespace inklink::draw
{
PolygonLineDrawingWidget::PolygonLineDrawingWidget(QWidget* parent) : QWidget(parent)
{
    setMouseTracking(true); // Включаем отслеживание движения мыши
}

// void PolygonLineDrawingWidget::paintEvent(QPaintEvent* event) {
//     QPainter painter(this);
//     painter.setRenderHint(QPainter::Antialiasing); // Сглаживание
//
//     // Рисуем все сохраненные прямые линии
//     painter.setPen(Qt::black);
//
//     for (const auto& line: lines) {
//         painter.drawPolygon(line);
//     }
//
//     // Рисуем текущую прямую (если есть)
//     if (!currentLine.isEmpty()) {
//         painter.drawPolygon(currentLine);
//     }
// }

void PolygonLineDrawingWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        // Создаем новую прямую и добавляем начальную точку
        currentLine.clear();
        currentLine << event->pos();
        currentLine << event->pos();
    }
}

void PolygonLineDrawingWidget::mouseMoveEvent(QMouseEvent* event)
{
    // Обновляем конечную точку текущей прямой при перемещении мыши
    if (event->buttons()&  Qt::LeftButton)
    {
        currentLine[1] = event->pos();
        update(); // Перерисовываем холст
    }
}

void PolygonLineDrawingWidget::mouseReleaseEvent(QMouseEvent* event)
{
    // Завершаем рисование текущей прямой при отпускании кнопки мыши
    if (event->button() == Qt::LeftButton)
    {
        if (currentLine.size() == 2)
        {
            lines.push_back(currentLine);
            currentLine.clear();
            update(); // Перерисовываем холст
        }
    }
}
} // namespace inklink::draw
