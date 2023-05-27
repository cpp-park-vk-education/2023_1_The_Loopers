#pragma once

#include <QApplication>
#include <QLine>
#include <QMouseEvent>
#include <QPainter>
#include <QVariant>
#include <QWidget>

#include <vector>

namespace inklink::draw
{
class LineWidget : public QWidget
{
public:
    LineWidget(QWidget* parent = nullptr);

protected:
    //    void paintEvent(QPaintEvent* event) override;

    void mousePressEvent(QMouseEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;

private:
    std::vector<QVariant> m_items; // Коллекция элементов (линий и прямоугольников)
    QPoint m_startPoint;           // Начальная точка
    QPoint m_endPoint;             // Конечная точка
};
} // namespace inklink::draw