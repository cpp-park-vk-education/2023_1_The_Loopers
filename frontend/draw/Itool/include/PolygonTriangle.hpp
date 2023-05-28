#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QPolygon>
#include <QWidget>

#include <vector>

namespace inklink::draw
{
class PolygonTriangleDrawingWidget : public QWidget
{
public:
    PolygonTriangleDrawingWidget(QWidget* parent = nullptr);

protected:
    //    void paintEvent(QPaintEvent* event) override;

    void mousePressEvent(QMouseEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;

    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    std::vector<QPolygon> triangles; // Коллекция треугольников
    QPolygon currentTriangle;        // Текущий треугольник
};
} // namespace inklink::draw
