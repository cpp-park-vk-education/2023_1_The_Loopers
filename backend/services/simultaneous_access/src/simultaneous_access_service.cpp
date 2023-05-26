#include "simultaneous_access_service.h"

#include "blocking_resolvers_factory.h"
#include "idraw_conflict_resolver.h"
#include "inklink_global.h"
#include "itext_conflict_resolver.h"

#include <data_container.h>
#include <iauthorizer.h>
#include <idb_adapter.h>
#include <iexternal_service_chassis.h>
#include <inklink/chassis_configurators/base_websocket_configurator.h>
#include <websocket_session_factory.h>

#include <json_serializer.h>

#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/system/error_code.hpp>

#include <chrono>
#include <filesystem>
#include <format>
#include <iostream>
#include <memory>
#include <string_view>
#include <thread>


namespace
{
namespace net = boost::asio;
using tcp = net::ip::tcp;

using error_code = boost::system::error_code;

using InternalSessionsManager = inklink::base_service_chassis::InternalSessionsManager;

using IAuthorizer = inklink::authorizer::IAuthorizer;
using IServiceSession = inklink::server_network::IServiceSession;

using DataContainer = inklink::serializer::DataContainer;
using JsonSerializer = inklink::serializer::JsonSerializer;

// is it ok to use define for such things? With define you can just write LOG_PATH"file_name.txt"
// and constexpr string is not possible in most cases. And string_view is not a lot more convenient (as far as I know)
constexpr std::string_view kLogPathPrefix = "inklink/simultaneous_access/";

// #if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
// constexpr const char* kLogPathPrefix = "";
// #else
// constexpr const char* kLogPathPrefix = "/var/log/";
// #endif

class InvalidMessageFormat
{
};

bool IsDrawAction(int msgActionType)
{
    switch (msgActionType)
    {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 12:
    case 13:
        return true;
    default:
        return false;
    }
}

inklink::ResolverActionType GetActionType(int msgActionType)
{
    using ResolverActionType = inklink::ResolverActionType;
    switch (msgActionType)
    {
    case 1:
        return ResolverActionType::kSelect;
    case 2:
        return ResolverActionType::kDeselect;
    case 3:
    case 4:
    case 6:
    case 7:
    case 12:
        return ResolverActionType::kInsertion;
    case 5:
    case 10:
    case 13:
        return ResolverActionType::kDeletion;
    case 8:
    case 11:
        return ResolverActionType::kFormat;
    default:
        throw InvalidMessageFormat();
    }
}
} // namespace

namespace inklink::service_simultaneous_access
{
int SimultaneousAccessService::Run()
{
    m_factory = std::make_unique<BlockingResolversFactory>();
    boost::asio::io_context ioContext;

    m_chassis = std::make_unique<external_service_chassis::IExternalServiceChassis>();

    auto manager = std::make_shared<InternalSessionsManager>();
    auto authorizer = std::make_shared<IAuthorizer>();

    auto onReadFunctor = [this](const std::string& str, error_code ec, IServiceSession* iss)
    { DoOnRead(str, ec, iss); };
    auto onConnectFunctor = [this](error_code ec, IServiceSession* iss) { DoOnConnect(ec, iss); };
    auto onWriteFunctor = [this](error_code ec, IServiceSession* iss) { DoOnWrite(ec, iss); };

    auto factory = std::make_unique<server_network::WebsocketSessionsFactory<
            decltype(onReadFunctor), decltype(onConnectFunctor), decltype(onWriteFunctor)>>( // I think, it's ok with
                                                                                             // default template params
            manager, authorizer, onReadFunctor, onConnectFunctor, onWriteFunctor);

    // TODO (a.novak) intellisense gives errors, but should not. Test with build in future (for now there is
    // dependencies on files from not merged prs)
    std::filesystem::create_directories(std::filesystem::path(kLogPathPrefix));
    // TODO (a.novak) add time to file name. For some reason, std format does not work
    //     const auto startTime = std::chrono::system_clock::now();
    const std::string logPath{std::string(kLogPathPrefix) + "simultaneous_access_.txt"};
    //      + std::format("{:%Y_%m_%d_%H_%M}", startTime) + ".txt"};
    // clang-format off
    m_chassis->baseServiceChassis = chassis_configurator::BaseChassisWebsocketConfigurator::CreateAndInitializeFullChassis(
            "simultaneous access", logPath, 
            ioContext, std::move(factory), manager, 
            ServiceType::kSimultaneousAccess, {.address = m_address, .port = m_port},
            [this](int eventType, const std::string& msgBody, Endpoint from)
                { DoOnNotified(eventType, msgBody, from); },
            [this](const std::string& msgBody) { DoOnSignal(msgBody); });
    // clang-format on
    m_chassis->baseServiceChassis->logger->LogInfo("Simultaneous access service is initted");
    std::cout << "Simultaneous access is initted" << __LINE__ << std::endl;

    // for now only one thread is supported
    ioContext.run();
    return 0;
}

void SimultaneousAccessService::DoOnRead(const std::string& msg, error_code ec, IServiceSession* sessionFrom)
{
    std::cout << "Read" << msg << __LINE__ << std::endl;
    if (ec)
    {
        m_chassis->baseServiceChassis->logger->LogDebug(std::string("Got error from ... while reading. Error: ") +
                                                        ec.what());

        return;
    }

    auto msgData = JsonSerializer::ParseFromString(msg);
    try
    {
        if (!msgData.Has("document_id") || !msgData.Has("action") || msgData.Has("time") || msgData.Has("figure_id"))
        {
            throw InvalidMessageFormat();
        }

        if (msgData["action"].AsInt("action_type") == 0 /*auth*/)
        {
            HandleNewUser(msgData, sessionFrom);
        }
        else if (msgData["action"].AsInt("action_type") == 14 /*end of session*/)
        {
            HandleUserExit(msgData);
        }
        else if (IsDrawAction(msgData["action"].AsInt("action_type")))
        {
            HandleDraw(msgData, sessionFrom);
        }
        else
        {
            HandleText(msgData, sessionFrom);
        }
    }
    catch (const InvalidMessageFormat&)
    {
        m_chassis->baseServiceChassis->logger->LogWarning(std::string("Got message of invalid format: ") + msg);
    }
}

void SimultaneousAccessService::DoOnConnect(error_code ec, IServiceSession* session)
{
    std::cout << "Connected" << session->GetClientEndpoint().address << __LINE__ << std::endl;
    if (ec)
    {
        std::cout << "Error occured" << ec.what() << __LINE__ << std::endl;
        m_chassis->baseServiceChassis->logger->LogDebug(
                std::string("Got error from ... when tried to accept. Error: ") + ec.what());
        return;
    }
}

void SimultaneousAccessService::DoOnWrite(error_code ec, IServiceSession*)
{
    if (ec)
    {
        m_chassis->baseServiceChassis->logger->LogDebug(std::string("Got error from ... while writing. Error: ") +
                                                        ec.what());
        return;
    }
}

void SimultaneousAccessService::DoOnNotified(int, const std::string&, Endpoint)
{
    return; // no known events to handle
}

void SimultaneousAccessService::DoOnSignal(const std::string&)
{
    return; // no known signals to handle
}

void SimultaneousAccessService::HandleNewUser(const DataContainer& msgData, IServiceSession* sessionFrom)
{
    const auto& docId = msgData.AsString("document_id");

    if (!m_usersCount.contains(docId))
    {
        m_usersCount[docId] = 0;
    }
    ++m_usersCount[docId];

    if (!m_drawResolvers.contains(docId))
    {
        m_drawResolvers[docId] = std::shared_ptr<IDrawConflictResolver>(m_factory->GetDrawConflictResolver());
        m_textResolvers[docId] = std::shared_ptr<ITextConflictResolver>(m_factory->GetTextConflictResolver());
    }

    const auto drawActionsHistory = m_drawResolvers[docId]->GetHistory();
    const auto textActionsHistory = m_textResolvers[docId]->GetHistory();

    auto sendDrawData{ContainerFromVectorOfDrawActions(drawActionsHistory)};
    auto sendTextData{ContainerFromVectorOfTextActions(textActionsHistory)};

    DataContainer sendMessage{};
    sendMessage["draw_actions"] = sendDrawData;
    sendMessage["text_actions"] = sendTextData;

    sessionFrom->Send(JsonSerializer::SerializeAsString(sendMessage));
}

void SimultaneousAccessService::HandleUserExit(const DataContainer& msgData)
{
    const auto& docId = msgData.AsString("document_id");

    --m_usersCount[docId];
    if (m_usersCount[docId] == 0)
    {
        const auto drawActionsHistory = m_drawResolvers[docId]->GetHistory();
        const auto textActionsHistory = m_textResolvers[docId]->GetHistory();

        auto sendDrawData{ContainerFromVectorOfDrawActions(drawActionsHistory)};
        auto sendTextData{ContainerFromVectorOfTextActions(textActionsHistory)};

        DataContainer sendMessage{};
        sendMessage["draw_actions"] = sendDrawData;
        sendMessage["text_actions"] = sendTextData;

        const Endpoint sendTo{.address = msgData["receiver"].AsString("address"),
                              .port = static_cast<std::uint16_t>(msgData["receiver"].AsInt("port"))};

        m_chassis->baseServiceChassis->signalBroker->Send(JsonSerializer::SerializeAsString(sendMessage), sendTo);
        m_drawResolvers.erase(docId);
    }
}

void SimultaneousAccessService::HandleDraw(const DataContainer& msgData, IServiceSession* session)
{
    const auto& docId = msgData.AsString("document_id");

    DrawAction action{};
    action.type = GetActionType(msgData["action"].AsInt("action_type"));
    action.time = std::chrono::system_clock::now();
    action.figureId = msgData.AsInt("figure_id");
    action.endpoint = session->GetClientEndpoint();
    action.data = msgData;

    if (!m_drawResolvers.contains(docId))
    {
        m_drawResolvers[docId] = std::shared_ptr<IDrawConflictResolver>(m_factory->GetDrawConflictResolver());
    }

    std::vector<DrawAction> actions{action};
    m_drawResolvers[docId]->Resolve(actions);

    const auto sessions = m_chassis->baseServiceChassis->internalSessionsManager->GetSessionsByDocument(docId);

    auto sendData{ContainerFromVectorOfDrawActions(actions)};

    for (const auto& session : sessions)
    {
        auto sessionLocked = session.lock();
        if (sessionLocked)
        {
            sessionLocked->Send(JsonSerializer::SerializeAsString(sendData));
        }
    }
}

void SimultaneousAccessService::HandleText(const DataContainer& msgData, IServiceSession* session)
{
    const auto& docId = msgData.AsString("document_id");

    TextAction action{};
    action.type = GetActionType(msgData["action"].AsInt("action_type"));
    action.time = std::chrono::system_clock::now();
    action.figureId = msgData.AsInt("figure_id");
    action.endpoint = session->GetClientEndpoint();
    action.posStart = msgData["action"]["action_description"].AsInt("start_position");
    action.posEnd = msgData["action"]["action_description"].AsInt("end_position");
    action.data = msgData;

    if (!m_textResolvers.contains(docId))
    {
        m_textResolvers[docId] = std::shared_ptr<ITextConflictResolver>(m_factory->GetTextConflictResolver());
    }

    std::vector<TextAction> actions{action};
    m_textResolvers[docId]->Resolve(actions);

    const auto sessions = m_chassis->baseServiceChassis->internalSessionsManager->GetSessionsByDocument(docId);

    auto sendData{ContainerFromVectorOfTextActions(actions)};

    for (const auto& session : sessions)
    {
        auto sessionLocked = session.lock();
        if (sessionLocked)
        {
            sessionLocked->Send(JsonSerializer::SerializeAsString(sendData));
        }
    }
}

DataContainer SimultaneousAccessService::ContainerFromVectorOfDrawActions(const std::vector<DrawAction>& actions)
{
    DataContainer container{};
    auto& dataArray = container.CreateArray();
    for (const auto& action : actions)
    {
        dataArray.push_back(action.data);
    }
    return container;
}

DataContainer SimultaneousAccessService::ContainerFromVectorOfTextActions(const std::vector<TextAction>& actions)
{
    DataContainer container{};
    auto& dataArray = container.CreateArray();
    for (const auto& action : actions)
    {
        dataArray.push_back(action.data);
    }
    return container;
}
} // namespace inklink::service_simultaneous_access
