#include "simultaneous_access_service.h"

#include "boost/asio/io_context.hpp"

#include <idb_adapter.h>
#include <iexternal_service_chassis.h>
#include <inklink/chassis_configurators/websocket_configurator.h>
#include <websocket_sessions_factory.h>

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

#include <chrono>
#include <filesystem>
#include <format>
#include <thread>

namespace
{
namespace net = boost::asio;
using tcp = net::ip::tcp;

using error_code = boost::system::error_code;

using InternalSessionsManager = inklink::base_service_chassis::InternalSessionsManager;

using IAuthorizer = inklink::server_network::IAuthorizer;
using IServiceSession = inklink::server_network::IServiceSession;

// is it ok to use define for such things? With define you can just write LOG_PATH"file_name.txt"
// and constexpr string is not possible in most cases. And string_view is not a lot more convenient (as far as I know)
constexpr const char* kLogPathPrefix = "inklink/simultaneous_access/";

// #if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
// constexpr const char* kLogPathPrefix = "";
// #else
// constexpr const char* kLogPathPrefix = "/var/log/";
// #endif
} // namespace

namespace inklink::service_simultaneous_access
{
int SimultaneousAccessService::Run()
{
    boost::asio::io_context ioContext;

    auto manager = std::make_shared<InternalSessionsManager>();
    auto auhorizer = std::make_shared<IAuthorizer>();
    auto factory = std::make_unique<WebsocketSessionsFactory>( // I think, it's ok with default template params
            manager, authorizer,
            [this](const std::string& str, error_code ec, IServiceSession* iss) { DoOnRead(str, ec, iss); },
            [this](error_code ec, IServiceSession* iss) { DoOnConnect(ec, iss); },
            [this](error_code ec, IServiceSession* iss) { DoOnWrite(ec, iss); });

    // TODO (a.novak) intellisense gives errors, but should not. Test with build in future (for now there is
    // dependencies on files from not merged prs)
    std::filesystem::create_directories(kLogPathPrefix);
    // TODO (a.novak) add time to file name. For some reason, std format does not work
    //     const auto startTime = std::chrono::system_clock::now();
    const std::string logPath{std::string(kLogPathPrefix) + "simultaneous_access_.txt"};
    //      + std::format("{:%Y_%m_%d_%H_%M}", startTime) + ".txt"};
    // clang-format off
    m_chassis = base_service_chassis::BaseChassisWebsocketConfigurator::CreateAndInitializeFullChassis(
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
    // TODO (a.novak) parse msg
    // resolve with appropriate resolver. Create one if does not exist and set max figure id
    std::string documentId;
    const auto sessions = m_chassis->baseServiceChassis->manager->GetSessionsByDocument(documentId);
    for (const auto& session : sessions)
    {
        session->Send("");
    }
}

void SimultaneousAccessService::DoOnConnect(error_code ec, IServiceSession* session)
{
    if (ec)
    {
        m_chassis->baseServiceChassis->logger->LogDebug(
                std::string("Got error from ... when tried to accept. Error: ") + ec.what());
    }
}

void SimultaneousAccessService::DoOnWrite(error_code ec, IServiceSession* session)
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
