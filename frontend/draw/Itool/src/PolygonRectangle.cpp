#include "polygon_rectangle.h"

namespace inklink::draw
{
PolygonRectangleDrawingWidget::PolygonRectangleDrawingWidget(QWidget* parent) : QWidget(parent)
{
    setMouseTracking(true); // Включаем отслеживание движения мыши
}

// void PolygonRectangleDrawingWidget::paintEvent(QPaintEvent* event) {
//     QPainter painter(this);
//     painter.setRenderHint(QPainter::Antialiasing); // Сглаживание
//
//     // Рисуем все сохраненные полигоны
//     painter.setPen(Qt::black);
//
//     for (const auto &polygon: polygons) {
//         painter.drawPolygon(polygon);
//     }
//
//     // Рисуем текущий полигон (если есть)
//     if (!currentPolygon.isEmpty()) {
//         painter.drawPolygon(currentPolygon);
//     }
// }

void PolygonRectangleDrawingWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        // Создаем новый полигон и добавляем начальную точку
        currentPolygon.clear();
        currentPolygon << event->pos();
    }
}

void PolygonRectangleDrawingWidget::mouseMoveEvent(QMouseEvent* event)
{
    // Обновляем текущий полигон при перемещении мыши
    if (event->buttons() & Qt::LeftButton)
    {
        if (currentPolygon.size() == 1)
        {
            // Добавляем первую вершину полигона
            currentPolygon << event->pos();
        }
        else if (currentPolygon.size() == 2)
        {
            // Заменяем вторую вершину полигона
            currentPolygon[1] = event->pos();
        }

        update(); // Перерисовываем холст
    }
}

void PolygonRectangleDrawingWidget::mouseReleaseEvent(QMouseEvent* event)
{
    // Завершаем рисование текущего полигона при отпускании кнопки мыши
    if (event->button() == Qt::LeftButton)
    {
        if (currentPolygon.size() == 2)
        {
            // Создаем прямоугольник из полигона
            QRect rectangle = QRect(currentPolygon[0], currentPolygon[1]).normalized();

            // Создаем полигон, представляющий прямоугольник
            QPolygon polygon;
            polygon.setPoints(4, rectangle.left(), rectangle.top(), rectangle.right(), rectangle.top(),
                              rectangle.right(), rectangle.bottom(), rectangle.left(), rectangle.bottom());

            polygons.push_back(polygon);
            currentPolygon.clear();
            update(); // Перерисовываем холст
        }
    }
}
} // namespace inklink::draw
