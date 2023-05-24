#include "simultaneous_access_service.h"

#include "blocking_resolvers_factory.h"
#include "data_wrapper.h"
#include "idraw_conflict_resolver.h"
#include "inklink_global.h"
#include "json_data.h"

#include <data_container.h>
#include <idb_adapter.h>
#include <iexternal_service_chassis.h>
#include <inklink/chassis_configurators/websocket_configurator.h>
#include <websocket_sessions_factory.h>

#include <json_serializer.h>

#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/system/error_code.hpp>

#include <chrono>
#include <filesystem>
#include <format>
#include <memory>
#include <string_view>
#include <thread>

namespace
{
namespace net = boost::asio;
using tcp = net::ip::tcp;

using error_code = boost::system::error_code;

using InternalSessionsManager = inklink::base_service_chassis::InternalSessionsManager;

using IAuthorizer = inklink::server_network::IAuthorizer;
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
    auto auhorizer = std::make_shared<IAuthorizer>();
    auto factory = std::make_unique<WebsocketSessionsFactory>( // I think, it's ok with default template params
            manager, authorizer,
            [this](const std::string& str, error_code ec, IServiceSession* iss) { DoOnRead(str, ec, iss); },
            [this](error_code ec, IServiceSession* iss) { DoOnConnect(ec, iss); },
            [this](error_code ec, IServiceSession* iss) { DoOnWrite(ec, iss); });

    // TODO (a.novak) intellisense gives errors, but should not. Test with build in future (for now there is
    // dependencies on files from not merged prs)
    std::filesystem::create_directories(std::filesystem::path(kLogPathPrefix));
    // TODO (a.novak) add time to file name. For some reason, std format does not work
    //     const auto startTime = std::chrono::system_clock::now();
    const std::string logPath{std::string(kLogPathPrefix) + "simultaneous_access_.txt"};
    //      + std::format("{:%Y_%m_%d_%H_%M}", startTime) + ".txt"};
    // clang-format off
    m_chassis->baseServiceChassis = base_service_chassis::BaseChassisWebsocketConfigurator::CreateAndInitializeFullChassis(
            "simultaneous access", logPath, 
            ioContext, std::move(factory), manager, 
            ServiceType::kSimultaneousAccess, {.address = m_address, .port = m_port},
            [this](int eventType, const std::string& msgBody, Endpoint from)
                { DoOnNotified(eventType, msgBody, from); },
            [this](const std::string& msgBody) { DoOnSignal(msgBody); });
    // clang-format on
    m_chassis->baseServiceChassis->logger->LogInfo("Simultaneous access service is initted");

    // for now only one thread is supported
    ioContext.run();
    return 0;
}

void SimultaneousAccessService::DoOnRead(const std::string& msg, error_code ec, IServiceSession* session)
{
    if (ec)
    {
        m_chassis->baseServiceChassis->logger->LogDebug(std::string("Got error from ... while reading. Error: ") +
                                                        ec.what());
    }

    auto msgData = JsonSerializer::ParseString(msg);
    try
    {
        if (!msgData.Has("document_id") || !msgData.Has("action") || msgData.Has("time") || msgData.Has("figure_id"))
        {
            throw InvalidMessageFormat();
        }

        const auto& docId = msgData.AsString("document_id");

        if (msgData["action"].AsInt("action_type") == 0 /*auth*/)
        {
            if (!m_usersCount.contains(docId))
            {
                m_usersCount[docId] = 0;
            }
            ++m_usersCount[docId];

            if (!m_drawResolvers.contains(docId))
            {
                m_drawResolvers[docId] = std::shared_ptr(m_factory->GetDrawConflictResolver());
            }

            const auto actionsHistory = m_drawResolvers[docId]->GetHistory();

            const auto sessions = m_chassis->baseServiceChassis->manager->GetSessionsByDocument(docId);
            for (const auto& session : sessions)
            {
                for (const auto& action : actionsHistory)
                {
                    session->Send(JsonSerializer::SerializeAsString(action.data));
                }
            }
        }
        else if (msgData["action"].AsInt("action_type") == 14 /*end of session*/)
        {
            --m_usersCount[docId];
            if (m_usersCount[docId] == 0)
            {
                const auto actionsHistory = m_drawResolvers[docId]->GetHistory();
                // TODO (a.novak) send signal with actions

                m_drawResolvers.erase(docId);
            }
        }
        else if (IsDrawAction(msgData["action"].AsInt("action_type")))
        {
            DrawAction action{};
            action.type = GetActionType(msgData["action"].AsInt("action_type"));
            action.time = std::chrono::system_clock::now();
            action.figureId = msgData.AsInt("figure_id");
            action.endpoint = session->GetClientEndpoint();
            action.data = msgData;

            // TODO delete (should not accept if not authorized)
            if (!m_drawResolvers.contains(docId))
            {
                m_drawResolvers.push_back(std::shared_ptr(m_factory->GetDrawConflictResolver()));
            }

            const auto resolvedActions = m_drawResolvers[docId]->Resolve({action});

            const auto sessions = m_chassis->baseServiceChassis->manager->GetSessionsByDocument(docId);
            for (const auto& session : sessions)
            {
                for (const auto& action : resolvedActions)
                {
                    session->Send(JsonSerializer::SerializeAsString(action.data));
                }
            }
        }
        else
        {
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
                m_textResolvers.push_back(std::shared_ptr(m_factory->GetTextConflictResolver()));
            }

            const auto resolvedActions = m_textResolvers[docId]->Resolve({action});

            const auto sessions = m_chassis->baseServiceChassis->manager->GetSessionsByDocument(docId);
            for (const auto& session : sessions)
            {
                for (const auto& action : resolvedActions)
                {
                    session->Send(JsonSerializer::SerializeAsString(action.data));
                }
            }
        }
    }
    catch (const InvalidMessageFormat&)
    {
        m_chassis->baseServiceChassis->logger->LogWarning(std::string("Got message of invalid format: ") + msg);
    }
}

void SimultaneousAccessService::DoOnConnect(error_code ec, IServiceSession*)
{
    if (ec)
    {
        m_chassis->baseServiceChassis->logger->LogDebug(
                std::string("Got error from ... when tried to accept. Error: ") + ec.what());
    }
}

void SimultaneousAccessService::DoOnWrite(error_code ec, IServiceSession*)
{
    if (ec)
    {
        m_chassis->baseServiceChassis->logger->LogDebug(std::string("Got error from ... while writing. Error: ") +
                                                        ec.what());
    }
}

void DoOnNotified(int, const std::string&, Endpoint)
{
    // TODO (a.novak)
}

void DoOnSignal(const std::string&)
{
    // TODO (a.novak)
}
} // namespace inklink::service_simultaneous_access
