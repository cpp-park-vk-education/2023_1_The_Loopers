#include "PolygonTriangle.hpp"

namespace inklink::draw
{
PolygonTriangleDrawingWidget::PolygonTriangleDrawingWidget(QWidget* parent)
{
    setMouseTracking(true); // Включаем отслеживание движения мыши
}

// void PolygonTriangleDrawingWidget::paintEvent(QPaintEvent* event) {
//     QPainter painter(this);
//     painter.setRenderHint(QPainter::Antialiasing); // Сглаживание
//
//     // Рисуем все сохраненные треугольники
//     painter.setPen(Qt::black);
//
//     for (const auto& triangle: triangles) {
//         painter.drawPolygon(triangle);
//     }
//
//     // Рисуем текущий треугольник (если есть)
//     if (!currentTriangle.isEmpty()) {
//         painter.drawPolygon(currentTriangle);
//     }
// }

void PolygonTriangleDrawingWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        // Создаем новый треугольник и добавляем начальную точку
        currentTriangle.clear();
        currentTriangle << event->pos();
    }
}

void PolygonTriangleDrawingWidget::mouseMoveEvent(QMouseEvent* event)
{
    // Обновляем текущий треугольник при перемещении мыши
    if (event->buttons() & Qt::LeftButton)
    {
        if (currentTriangle.size() < 3)
        {
            // Добавляем точку к текущему треугольнику
            currentTriangle << event->pos();
        }
        else
        {
            // Заменяем последние две точки треугольника
            currentTriangle[currentTriangle.size() - 2] = event->pos();
            currentTriangle[currentTriangle.size() - 1] = event->pos() + QPoint(100, 0); // Изменяем размер основания
        }

        update(); // Перерисовываем холст
    }
}

void PolygonTriangleDrawingWidget::mouseReleaseEvent(QMouseEvent* event)
{
    // Завершаем рисование текущего треугольника при отпускании кнопки мыши
    if (event->button() == Qt::LeftButton)
    {
        if (currentTriangle.size() == 3)
        {
            triangles.push_back(currentTriangle);
            currentTriangle.clear();
            update(); // Перерисовываем холст
        }
    }
}
} // namespace inklink::draw
