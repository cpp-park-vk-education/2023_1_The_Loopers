#include "DrawView.h"

namespace inklink::draw
{
void DrawView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        drawing = true;
        lastPoint = event->pos();
    }
    if (event->button() == Qt::RightButton)
    {
        if (!objects.empty())
        {
            for (auto &object : objects)
            {
                if (object->contains(event->pos()))
                {
                    selectedObject = object;
                    lastOffset = event->pos() - object->pos();
                    break;
                }
            }
        }
    }
}
} // namespace inklink::draw