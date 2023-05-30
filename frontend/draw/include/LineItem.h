#pragma once

#include "DataContainer.h"

#include <QGraphicsObject>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>

class LineItem : public QGraphicsObject
{
public:
    LineItem(QGraphicsItem *parent = nullptr)
            : QGraphicsObject(parent), m_selected(false), m_selectedPoint(-1)
    {
        setFlag(ItemIsSelectable);
        setAcceptHoverEvents(true);
        setZValue(1); // Ensure the line is drawn on top of other items
    }

    QRectF boundingRect() const override
    {
        return m_line.boundingRect();
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override
    {
        Q_UNUSED(widget);

        painter->setRenderHint(QPainter::Antialiasing);

        if (option->state & QStyle::State_Selected)
            painter->setPen(Qt::red);
        else
            painter->setPen(Qt::black);

        painter->drawLine(m_line);
        drawPoints(painter);
    }

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton)
        {
            const QPointF pos = event->pos();
            const qreal pointSize = m_pointSize / 2;
            const QRectF startRect(m_line.p1() - QPointF(pointSize, pointSize), QSizeF(m_pointSize, m_pointSize));
            const QRectF endRect(m_line.p2() - QPointF(pointSize, pointSize), QSizeF(m_pointSize, m_pointSize));

            if (startRect.contains(pos))
            {
                m_selected = true;
                m_selectedPoint = 0;
                return;
            }

            if (endRect.contains(pos))
            {
                m_selected = true;
                m_selectedPoint = 1;
                return;
            }

            m_selected = true;
            m_selectedPoint = -1;
        }
    }

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override
    {
        if (m_selected)
        {
            if (m_selectedPoint == 0)
            {
                m_line.setP1(event->pos());
            }
            else if (m_selectedPoint == 1)
            {
                m_line.setP2(event->pos());
            }
            else
            {
                const QPointF delta = event->pos() - event->lastPos();
                m_line.translate(delta);
            }

            prepareGeometryChange();
            update();
        }
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton)
        {
            m_selected = false;
            m_selectedPoint = -1;

            DataContainer polygonInfo;

            polygonInfo["number of angles"] = 2;

            std::vector<DataContainer> anglesArray;

            QPointF startPoint = m_line.p1();
            QPointF endPoint = m_line.p2();

            DataContainer startPointContainer;
            startPointContainer["x"] = startPoint.x();
            startPointContainer["y"] = startPoint.y();

            DataContainer endPointContainer;
            endPointContainer["x"] = endPoint.x();
            endPointContainer["y"] = endPoint.y();

            anglesArray.push_back(startPointContainer);
            anglesArray.push_back(endPointContainer);

            polygonInfo["angles coordinates"] = anglesArray;

//            std::cout << polygonInfo << std::endl;
        }
    }

private:
    QPainterPath shape() const override
    {
        QPainterPath path;
        path.moveTo(m_line.p1());
        path.lineTo(m_line.p2());
        return path;
    }

    void drawPoints(QPainter *painter) const
    {
        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::black);

        const qreal pointSize = m_pointSize / 2;
        const QRectF startRect(m_line.p1() - QPointF(pointSize, pointSize), QSizeF(m_pointSize, m_pointSize));
        const QRectF endRect(m_line.p2() - QPointF(pointSize, pointSize), QSizeF(m_pointSize, m_pointSize));

        painter->drawEllipse(startRect);
        painter->drawEllipse(endRect);
    }

    QLineF m_line;
    bool m_selected;
    int m_selectedPoint;
    const qreal m_pointSize = 8.0;
};
