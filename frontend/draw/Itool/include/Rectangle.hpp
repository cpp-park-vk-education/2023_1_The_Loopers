#pragma once

#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QVariant>
#include <QWidget>

#include <vector>

namespace inklink::draw
{
class RectangleDrawingWidget : public QWidget
{
public:
    RectangleDrawingWidget(QWidget* parent = nullptr);

protected:
    //    void paintEvent(QPaintEvent* event) override;

    void mousePressEvent(QMouseEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;

private:
    std::vector<QVariant> items; // Коллекция элементов (прямоугольников)
    QPoint startPoint;           // Начальная точка
    QPoint endPoint;             // Конечная точка
};
} // namespace inklink::draw