#include "IObject.h"

#include <json_serializer.h>

namespace
{
using JsonSerializer = inklink::serializer::JsonSerializer;
} // namespace

namespace inklink::draw
{

void ObjectWithAttributes::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (m_selected) // do nothing
    {
        return;
    }
    QGraphicsObject::mousePressEvent(event);

    setSelected(false);
    m_selected = true;

    DataContainer action{};

    action["action_id"] = m_dis(m_gen);
    action["action_type"] = 1; // select

    emit Changed(JsonSerializer::SerializeAsString(action).c_str());
}

std::string TextBox::serialize()
{
    return "";
}

void TextBox::parse(const DataContainer& msgData)
{
    if (!isMsgValid(msgData))
        return;

    if (msgData.AsInt("action_type") == 1) // select
    {
        setSelected(true);
        return;
    }

    if (msgData.AsInt("action_type") == 2) // deselect
    {
        setSelected(false);
        m_selected = false;
        return;
    }
}

void TextBox::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
}

void TextBox::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (!isSelected()) // did not get approve from server, but user released
    {
        QGraphicsObject::mouseReleaseEvent(event);
        return;
    }

    // serialize all history (and emit one by one)

    DataContainer action{};
    action["action_id"] = m_dis(m_gen);
    action["action_type"] = 2; // deselect

    QGraphicsObject::mouseReleaseEvent(event);
    m_selected = false;
}

std::string Polygon::serialize()
{
    return "";
}

void Polygon::parse(const DataContainer& msgData)
{
    if (!isMsgValid(msgData))
        return;

    if (msgData.AsInt("action_type") == 1) // select
    {
        setSelected(true);
        return;
    }

    if (msgData.AsInt("action_type") == 2) // deselect
    {
        setSelected(false);
        m_selected = false;
        return;
    }

    if (msgData.AsInt("action_type") == 5) // polygon move
    {
        // this->coord = newCoord;
    }
}

void Polygon::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
}

void Polygon::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
}

std::string Ellipse::serialize()
{
    return "";
}

void Ellipse::parse(const DataContainer&)
{
}

void Ellipse::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
}

void Ellipse::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
}

} // namespace inklink::draw
