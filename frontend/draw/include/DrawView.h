#pragma once

#include <QKeyEvent>
#include <QWidget>

namespace inklink::draw
{
class DrawView : public QWidget
{
    Q_OBJECT

public:
    DrawView(QWidget* parent = nullptr);

    void NotifyGotResultFromNetwork(bool result);

signals:
    void GotResultFromNetwork(bool result);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    void DoOnGotResultFromNetwork(bool result)
    {
    }

private:
    QList<QPolygonF> m_polygons;
    QPolygonF m_currentLine;
    QPolygonF* m_selectedPolygon = nullptr;
    QPointF m_offset;
};
} // namespace inklink::draw