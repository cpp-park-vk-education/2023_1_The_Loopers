#include "DrawView.h"

#include <QPainter>
#include <QWidget>

namespace{
constexpr QColor kBackgroundColor(30,30,30);
constexpr QColor kDotsColor(218, 218, 218);
constexpr int kPointSize = 70;
}

namespace inklink::draw
{
DrawView::DrawView(QWidget* parent) : QWidget(parent)
{
    resize(1280, 720);
    setAutoFillBackground(true);
}

void DrawView::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_currentLine.clear();
        m_currentLine.append(event->pos());
    }
    if (event->button() == Qt::RightButton)
    {
        for (QPolygonF& polygon : m_polygons)
        {
            if (polygon.boundingRect().contains(event->pos()))
            {
                m_selectedPolygon = &polygon;
                m_offset = polygon.boundingRect().topLeft() - event->pos();
                break;
            }
        }
    }
}

void DrawView::mouseMoveEvent(QMouseEvent* event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        m_currentLine.append(event->pos());
        update();
    }
    if (event->buttons() & Qt::RightButton && m_selectedPolygon != nullptr)
    {
        m_selectedPolygon->translate(event->pos() + m_offset - m_selectedPolygon->boundingRect().topLeft());
        update();
    }
}

void DrawView::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (!m_currentLine.isEmpty())
        {
            m_polygons.append(m_currentLine);
            m_currentLine.clear();
            update();
        }
    }
    if (event->button() == Qt::RightButton)
    {
        m_selectedPolygon = nullptr;
    }
}

void DrawView::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    painter.fillRect(rect(), kBackgroundColor); // Background color | should be variable

    painter.setPen(QPen(kDotsColor, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.setRenderHint(QPainter::Antialiasing);

    for (int x = kPointSize /2; x < width(); x += kPointSize)
    {
        for (int y = kPointSize /2; y < height(); y += kPointSize)
        {
            painter.drawPoint(x, y);
        }
    }

    for (const QPolygonF& polygon : m_polygons)
    {
        painter.drawPolyline(polygon);
    }

    if (!m_currentLine.isEmpty())
    {
        painter.setPen(Qt::blue);
        painter.drawPolyline(m_currentLine);
    }

    if (m_selectedPolygon != nullptr)
    {
        painter.setPen(Qt::red);
        painter.drawRect(m_selectedPolygon->boundingRect());
    }
}

} // namespace inklink::draw
