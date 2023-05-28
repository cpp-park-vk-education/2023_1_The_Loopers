#pragma once

#include <QWidget>
#include <QKeyEvent>

namespace inklink::draw
{
class DrawView : public QWidget
{
public:
    DrawView(QWidget* parent = nullptr);

    void NotifyGotResultFromNetwork(auto result);

signals:
    void GotResultFromNetwork(auto result);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    void DoOnGotResultFromNetwork(auto result) {}

private:
    QList<QPolygonF> m_polygons;
    QPolygonF m_currentLine;
    QPolygonF* m_selectedPolygon = nullptr;
    QPointF m_offset;
};
} // namespace inklink::draw
