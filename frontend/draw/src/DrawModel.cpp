#include "DrawModel.h"

#include "DrawView.h"
#include "data_container.h"
#include "json_serializer.h"
#include "websocket_client_session.h" // Sasha Novak says it should be in <> scopes, but i don't really know

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
DrawModel::DrawModel()
{
    {
        auto lambdaOnAccept = [this](ConnectType, error_code ec, IClientSession*) { ; };
        auto lambdaOnRead = [this](const std::string& str, error_code ec, IClientSession*) { this->ParseToGet(str); };
        auto accessSession = std::make_shared<WebsocketClientSession<decltype(lambdaOnAccept), decltype(lambdaOnRead)>>(
                m_ioContext, lambdaOnAccept, lambdaOnRead);
        auto storageSession =
                std::make_shared<WebsocketClientSession<decltype(lambdaOnAccept), decltype(lambdaOnRead)>>(
                        m_ioContext, lambdaOnAccept, lambdaOnRead);
        accessSession->RunAsync("127.0.0.1", simultaneousAccess);
        storageSession->RunAsync("127.0.0.1", fileStorage);
        m_accessSession = std::move(accessSession);
        m_storageSession = std::move(storageSession);

        m_ioContextExecutor =
                boost::asio::require(m_ioContext.get_executor(), boost::asio::execution::outstanding_work.tracked);

        m_threadIoContext = std::thread([this]() { this->m_ioContext.run(); });
    }
}

std::string DrawModel::Serialize(int actionType, int figureId, int type)
{
    DataContainer sendContainer{};
    sendContainer["document_id"] = m_filename;
    auto action = sendContainer["action"];
    action["action_id"] = GenerateRandomNumber();
    action["action_type"] = actionType;
    action["action_description"]["type"] = type;
    auto actionInfo = action["action_description"]["info"];

    // in this block we specify filling of container according to type of actionInfo
    if (type == kSelection){
        actionInfo["figure_id"] = figureId;
    }
    if (type == kPolygon)
    {
        actionInfo["number_of_angles"];
        auto& anglesArray = actionInfo["angles_coordinates"].CreateArray();
        DataContainer vertex;
        for (auto& values : vectorOfCoordinates){
            vertex["x"] = values.x;
            vertex["y"] = values.y;
            anglesArray.push_back(vertex);
        }
    }
    if (type == kEllipse)
    {
        actionInfo["center"]["x"];
        actionInfo["center"]["y"];
        actionInfo["x_radius"];
        actionInfo["y_radius"];
    }
// text infos

    sendContainer["time"] = "now"; // for now time is not working
    sendContainer["figure_id"] = figureId;
    return JsonSerializer::SerializeAsString(sendContainer);
}

DrawModel::~DrawModel()
{
    m_ioContext.stop();
    m_threadIoContext.join();
}

void DrawModel::Send(std::string& message)
{
    std::shared_ptr<IClientSession> AccessSession = m_accessSession.lock();
    if (!AccessSession) [[unlikely]]
    {
        auto lambdaOnAccept = [this](ConnectType, error_code ec, IClientSession*) { ; };
        auto lambdaOnRead = [this](const std::string& str, error_code ec, IClientSession*) { this->Deserialize(str); };
        AccessSession = std::make_shared<WebsocketClientSession<decltype(lambdaOnAccept), decltype(lambdaOnRead)>>(
                m_ioContext, lambdaOnAccept, lambdaOnRead);
        AccessSession->RunAsync("127.0.0.1", simultaneousAccess);
        m_accessSession = AccessSession;
    }
    AccessSession->Send(message);
}

void DrawModel::SetFilename(std::string& filename)
{
    m_filename = filename;
}

void DrawModel::Deserialize(const std::string& message)
{
    DataContainer gotData = JsonSerializer::ParseFromString(message);
    m_view->NotifyGotResultFromNetwork(true);
    return;
}
} // namespace inklink::draw
