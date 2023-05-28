#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QPolygon>
#include <QWidget>

#include <vector>

namespace inklink::draw
{
class PolygonRectangleDrawingWidget : public QWidget
{
public:
    PolygonRectangleDrawingWidget(QWidget* parent = nullptr);

protected:
    //    void paintEvent(QPaintEvent* event) override;

    void mousePressEvent(QMouseEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;

    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    std::vector<QPolygon> polygons; // Коллекция полигонов
    QPolygon currentPolygon;        // Текущий рисуемый полигон
};
} // namespace inklink::draw
