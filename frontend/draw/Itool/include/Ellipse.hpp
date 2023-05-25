#pragma once

#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <vector>

class EllipseDrawingWidget : public QWidget {
public:
    EllipseDrawingWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

private:
    std::vector<QRect> ellipses; // Коллекция эллипсов
    QPoint startPoint; // Начальная точка
    QPoint endPoint; // Конечная точка
};