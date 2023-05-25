#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QWidget>

class RectangleDrawingWidget : public QWidget
{
public:
    RectangleDrawingWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

private:
    std::vector<QRect> rectangles; // Коллекция прямоугольников
    QPoint startPoint;             // Начальная точка
    QPoint endPoint;               // Конечная точка
};