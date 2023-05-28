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
        accessSession->RunAsync("127.0.0.1", 3997);  // 3997 simultaneous access
        storageSession->RunAsync("127.0.0.1", 3995); // 3995 file storage
        m_accessSession = std::move(accessSession);
        m_storageSession = std::move(storageSession);

        m_ioContextExecutor =
                boost::asio::require(m_ioContext.get_executor(), boost::asio::execution::outstanding_work.tracked);

        m_threadIoContext = std::thread([this]() { this->m_ioContext.run(); });
    }
}

std::string DrawModel::Serialize(int actionId, int actionType, int figureId)
{
    DataContainer sendContainer{};
    sendContainer["document_id"] = m_filename;
    sendContainer["action"]["action_id"] = actionId;
    sendContainer["action"]["action_type"] = actionType;
    sendContainer["time"] = "now";
    sendContainer["figure_id"] = figureId;
    return JsonSerializer::SerializeAsString(sendContainer);
}

void DrawModel::Send(std::string& message) {
    std::shared_ptr<IClientSession> AccessSession = m_accessSession.lock();
    if (!AccessSession) [[unlikely]]
    {
        auto lambdaOnAccept = [this](ConnectType, error_code ec, IClientSession*) {
            ;
        };
        auto lambdaOnRead = [this](const std::string& str, error_code ec, IClientSession*) {
            this->ParseToGet(str);
        };
        AccessSession = std::make_shared<WebsocketClientSession<
                decltype(lambdaOnAccept),
                decltype(lambdaOnRead)>>
                (m_ioContext, lambdaOnAccept, lambdaOnRead);
        AccessSession->RunAsync("127.0.0.1", 3997);
        m_accessSession = AccessSession;
    }
    AccessSession->Send(message);
}

void DrawModel::Deserialize(const std::string& message)
{
    DataContainer gotData = JsonSerializer::ParseFromString(message);
    m_view->NotifyGotResultFromNetwork(true);
    return;
}
} // namespace inklink::draw
