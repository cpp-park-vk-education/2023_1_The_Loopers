#pragma once

#include <QGraphicsView>
#include <QWheelEvent>

// TODO: (zakharov) add function to add vertex and edge to the view, make something with Q_OBJECT macros
namespace inklink::graph
{
class VertexView;

class GraphView : public QGraphicsView
{
    //    Q_OBJECT
public:
    GraphView(QWidget* parent = nullptr);

    ~GraphView() = default;

protected:
    void wheelEvent(QWheelEvent* event) override;
    void ScaleView(qreal scaleFactor);
    void drawBackground(QPainter* painter, const QRectF& rect) override;
};
} // namespace inklink::graph
