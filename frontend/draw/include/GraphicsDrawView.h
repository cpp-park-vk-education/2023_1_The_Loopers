#pragma once

#include <QApplication>
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>

namespace inklink::draw
{
class DrawSceneModel;

class GraphicsDrawView : public QGraphicsView
{
    Q_OBJECT

public:
    GraphicsDrawView(QWidget* parent = nullptr);

    DrawSceneModel* GetSceneModel()
    {
        return m_scene;
    }

    // protected:
    //     void mousePressEvent(QMouseEvent* event) override;
    //     void mouseReleaseEvent(QMouseEvent* event) override;
    //     void mouseMoveEvent(QMouseEvent* event) override;

private:
    DrawSceneModel* m_scene;

    bool drawing;
    QPointF startPoint;
    QPointF endPoint;
    QPointF lastPos;

    QGraphicsItem* movingItem;
    QPointF movingOffset;
};
} // namespace inklink::draw
