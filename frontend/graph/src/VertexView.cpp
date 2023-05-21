#include "VertexView.h"

namespace inklink::graph
{
VertexView::VertexView(GraphView *graphView) : m_graph(graphView)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(1);
}
} // namespace inklink::graph