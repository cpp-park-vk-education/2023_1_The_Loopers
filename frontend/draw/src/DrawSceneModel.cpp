#include "DrawSceneModel.h"

#include "GraphicsDrawView.h"
#include "IObject.h"
#include "all_items"

#include <data_container.h>
#include <websocket_client_session.h> // Sasha Novak says it should be in <> scopes, but i don't really know

#include <json_serializer.h>

#include <QGraphicsSceneMouseEvent>

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

#include <chrono>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

namespace
{
constexpr int simultaneousAccess = 3997;
constexpr int fileStorage = 3995;

using namespace std::chrono_literals;
using namespace inklink::client_connector;
using error_code = boost::system::error_code;

using DataContainer = inklink::serializer::DataContainer;
using JsonSerializer = inklink::serializer::JsonSerializer;

using IClientSession = inklink::client_connector::IClientSession;

enum actionInfoTypes
{
    kAuth,
    kSelection,
    kPolygon,
    kEllipse,
    kPixmap,
    kTextBlock,
    kTextInsert,
    kTextDelete,
    kTextFormat
};
} // namespace

namespace inklink::draw
{
DrawSceneModel::DrawSceneModel(QObject* parent)
        : QGraphicsScene(parent), m_gen{static_cast<unsigned long>(
                                          std::chrono::system_clock::now().time_since_epoch().count())}
{
    {
        auto lambdaOnAccept = [this](ConnectType, error_code ec, IClientSession*) { ; };
        auto lambdaOnRead = [this](const std::string& str, error_code ec, IClientSession*) { this->Deserialize(str); };
        auto accessSession = std::make_shared<
                inklink::client_connector::WebsocketClientSession<decltype(lambdaOnAccept), decltype(lambdaOnRead)>>(
                m_ioContext, lambdaOnAccept, lambdaOnRead);
        auto storageSession = std::make_shared<
                inklink::client_connector::WebsocketClientSession<decltype(lambdaOnAccept), decltype(lambdaOnRead)>>(
                m_ioContext, lambdaOnAccept, lambdaOnRead);
        accessSession->RunAsync("127.0.0.1", simultaneousAccess);
        storageSession->RunAsync("127.0.0.1", fileStorage);
        m_accessSession = accessSession;
        m_storageSession = storageSession;

        m_ioContextExecutor =
                boost::asio::require(m_ioContext.get_executor(), boost::asio::execution::outstanding_work.tracked);

        m_threadIoContext = std::thread([this]() { this->m_ioContext.run(); });
    }
}

void DrawSceneModel::SetMode(DrawSceneModel::Mode mode)
{
    m_currMode = mode;
}

void DrawSceneModel::SetLineMode()
{
    SetMode(Mode::kLine);
}

void DrawSceneModel::SetFreeLineMode()
{
    SetMode(Mode::kFreeLine);
}

void DrawSceneModel::SetEllipseMode()
{
    SetMode(Mode::kEllipse);
}

void DrawSceneModel::SetRectangleMode()
{
    SetMode(Mode::kRectangle);
}

std::string DrawSceneModel::Serialize(int actionType, int figureId, int type)
{
    DataContainer sendContainer{};
    sendContainer["document_id"] = m_filename;
    auto action = sendContainer["action"];
    action["action_id"] = GenerateRandomNumber();
    action["action_type"] = actionType;
    action["action_description"]["type"] = type;
    auto actionInfo = action["action_description"]["info"];

    // in this block we specify filling of container according to type of actionInfo
    if (type == kSelection)
    {
        actionInfo["figure_id"] = figureId;
    }
    if (type == kPolygon)
    {
        // auto currentPolygon = dynamic_cast<Polygon*>(m_objects[figureId]);
        // actionInfo["number_of_angles"] = static_cast<int>(currentPolygon->m_arrayOfVertexCoordinates.size());
        // auto& anglesArray = actionInfo["angles_coordinates"].CreateArray();
        // DataContainer vertex;
        // for (auto values : currentPolygon->m_arrayOfVertexCoordinates)
        // {
        //     vertex["x"] = values.xPosition;
        //     vertex["y"] = values.yPosition;
        //     anglesArray.push_back(vertex);
        // }
    }
    if (type == kEllipse)
    {
        // auto currentEllipse = dynamic_cast<Ellipse*>(m_objects[figureId]);
        // actionInfo["center"]["x"] = currentEllipse->m_center.xPosition;
        // actionInfo["center"]["y"] = currentEllipse->m_center.yPosition;
        // actionInfo["x_radius"] = currentEllipse->m_xRadius;
        // actionInfo["y_radius"] = currentEllipse->m_yRadius;
    }
    // currently working not properly, i think

    sendContainer["time"] = "now"; // for now time is not working
    sendContainer["figure_id"] = figureId;
    return JsonSerializer::SerializeAsString(sendContainer);
}

DrawSceneModel::~DrawSceneModel()
{
    m_ioContext.stop();
    m_threadIoContext.join();
}

void DrawSceneModel::Send(std::string& message)
{
    if (m_accessSession.expired() || !m_accessSession.lock()) [[unlikely]]
    {
        auto lambdaOnAccept = [this](ConnectType, error_code ec, IClientSession*) { ; };
        auto lambdaOnRead = [this](const std::string& str, error_code ec, IClientSession*) { this->Deserialize(str); };
        auto AccessSession = std::make_shared<WebsocketClientSession<decltype(lambdaOnAccept), decltype(lambdaOnRead)>>(
                m_ioContext, lambdaOnAccept, lambdaOnRead);
        AccessSession->RunAsync("127.0.0.1", simultaneousAccess);
        m_accessSession = AccessSession;
    }

    std::shared_ptr<IClientSession> AccessSession = m_accessSession.lock();
    AccessSession->Send(message);
}

void DrawSceneModel::SetFilename(std::string& filename)
{
    m_filename = filename;
}

void DrawSceneModel::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        // Create a new QGraphicsItem
        ObjectWithAttributes* newItem = CreateNewItem();

        if (!newItem)
        {
            QGraphicsScene::mouseDoubleClickEvent(event);
            return;
        }

        // Add the new item to the scene
        addItem(newItem);

        // Simulate a mouse press event on the newly created item
        QGraphicsSceneMouseEvent pressEvent(QEvent::GraphicsSceneMousePress);
        pressEvent.setScenePos(event->scenePos());
        pressEvent.setButton(Qt::LeftButton);
        newItem->mousePressEvent(&pressEvent);
    }

    QGraphicsScene::mouseDoubleClickEvent(event);
}

ObjectWithAttributes* DrawSceneModel::CreateNewItem()
{
    ObjectWithAttributes* newItem = nullptr;

    // Create the appropriate item based on the current mode
    switch (m_currMode)
    {
    case Mode::kMove:
        // No item to create for move mode
        break;
    case Mode::kLine:
        newItem = new LineItem(this);
        break;
    case Mode::kFreeLine:
        newItem = new FreeLineItem(this);
        break;
    case Mode::kRectangle:
        newItem = new RectangleItem(this);
        break;
    case Mode::kEllipse:
        newItem = new EllipseItem(this);
        break;
    }

    if (newItem)
    {
        addItem(newItem);
        m_itemsById[newItem->getID()] = newItem;
    }

    return newItem;
}

void DrawSceneModel::Deserialize(const std::string& message)
{
    DataContainer gotData = JsonSerializer::ParseFromString(message);
    //    m_view->NotifyGotResultFromNetwork(true);
    return;
}

int DrawSceneModel::GenerateRandomNumber()
{
    return m_dis(m_gen);
}
} // namespace inklink::draw
