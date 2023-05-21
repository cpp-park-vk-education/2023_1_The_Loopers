#include "VertexView.h"

namespace inklink::graph
{
VertexView::VertexView(GraphView *graphView) : graph(graphView)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(1);
}
} // namespace inklink::graph