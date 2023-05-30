#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QPolygon>
#include <QWidget>

#include <vector>

namespace inklink::draw
{
class PolygonLineDrawingWidget : public QWidget
{
public:
    PolygonLineDrawingWidget(QWidget* parent = nullptr);

protected:
    //    void paintEvent(QPaintEvent* event) override;

    void mousePressEvent(QMouseEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;

    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    std::vector<QPolygon> lines; // Коллекция прямых линий
    QPolygon currentLine;        // Текущая рисуемая прямая
};
} // namespace inklink::draw
