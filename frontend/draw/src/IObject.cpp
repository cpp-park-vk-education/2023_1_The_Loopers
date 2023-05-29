#include "IObject.h"

#include <json_serializer.h>

namespace
{
using JsonSerializer = inklink::serializer::JsonSerializer;
} // namespace

namespace inklink::draw
{

std::string TextBox::serialize()
{
    return "";
}

void TextBox::parse(const DataContainer &)
{
}

void TextBox::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_selected) // do nothing
    {
        return;
    }
    QGraphicsItem::mousePressEvent(event);

    setSelected(false);
    m_selected = true;

    DataContainer action{};

    action["action_id"] = m_dis(m_gen);
    action["action_type"] = 1; // select

    emit Changed(JsonSerializer::SerializeAsString(action).c_str());
}

void TextBox::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (!isSelected()) // did not get approve from server, but user released
    {
        QGraphicsItem::mouseReleaseEvent(event);
        return;
    }

    // serialize all history (and emit one by one)

    action["action_id"] = m_dis(m_gen);
    action["action_type"] = 2; // deselect

    QGraphicsItem::mouseReleaseEvent(event);
    m_selected = false;
}

std::string Polygon::serialize()
{
    return "";
}

void Polygon::parse(const DataContainer &)
{
}

void Polygon::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
}

void Polygon::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
}

std::string Ellipse::serialize()
{
    return "";
}

void Ellipse::parse(const DataContainer &)
{
}

void Ellipse::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
}

void Ellipse::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
}

} // namespace inklink::draw
