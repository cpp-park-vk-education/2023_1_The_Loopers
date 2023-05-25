#include "message_broker_service.h"

#include "data_container.h"

#include <data_container.h>
#include <iexternal_service_chassis.h>
#include <inklink/chassis_configurators/websocket_configurator.h>
#include <websocket_sessions_factory.h>

#include <json_serializer.h>

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

using DataContainer = inklink::serializer::DataContainer;
using JsonSerializer = inklink::serializer::JsonSerializer;

// is it ok to use define for such things? With define you can just write LOG_PATH"file_name.txt"
// and constexpr string is not possible in most cases. And string_view is not a lot more convenient (as far as I know)
constexpr const char* kLogPathPrefix = "inklink/message_broker/message_broker";

// #if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
// constexpr const char* kLogPathPrefix = "";
// #else
// constexpr const char* kLogPathPrefix = "/var/log/";
// #endif
} // namespace

namespace inklink::service_message_broker
{
int MessageBrokerService::Run()
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
    const std::string logPath{std::string(kLogPathPrefix) + "_.txt"};
    //      + std::format("{:%Y_%m_%d_%H_%M}", startTime) + ".txt"};
    // clang-format off
    m_chassis = base_service_chassis::BaseChassisWebsocketConfigurator::CreateAndInitializeChassisWithoutMsgBroker(
            "simultaneous access", logPath, 
            ioContext, std::move(factory), manager, 
            ServiceType::kMessageBroker, {.address = m_address, .port = m_port});
    // clang-format on
    m_chassis->logger->LogInfo("Simultaneous access service is initted");

    m_eventsHandler = std::make_unique<IEventsHandler>(*m_chassis);
    m_signalsHandler = std::make_unique<ISignalsHandler>(*m_chassis);

    // for now only one thread is supported
    ioContext.run();
    return 0;
}

void MessageBrokerService::DoOnRead(const std::string& msg, error_code ec, IServiceSession* session)
{
    if (ec)
    {
        m_chassis->logger->LogDebug(std::string("Got error while reading from '...'. Error: ") + ec.what());
    }
    const auto& msgData = JsonSerializer::ParseString(msg);
    if (msgData.Has("event"))
    {
        if (!m_eventsHandler->Handle(msgData, session->GetClientEndpoint()))
        {
            m_chassis->logger->LogDebug(std::string("Got message with invalid format: ") + msg);
        }
        return;
    }
    if (msgData.Has("receiver"))
    {
        if (!m_signalsHandler->Handle(msgData, session->GetClientEndpoint()))
        {
            m_chassis->logger->LogDebug(std::string("Got message with invalid format: ") + msg);
        }
        return;
    }
    m_chassis->logger->LogDebug(std::string("Got message with invalid format: ") + msg);
}

void MessageBrokerService::DoOnConnect(error_code ec, IServiceSession*)
{
    if (ec)
    {
        m_chassis->logger->LogDebug(std::string("Got error from ... when tried to accept. Error: ") + ec.what());
    }
}

void MessageBrokerService::DoOnWrite(error_code ec, IServiceSession*)
{
    if (ec)
    {
        m_chassis->logger->LogDebug(std::string("Got error from ... while writing. Error: ") + ec.what());
    }
}
} // namespace inklink::service_message_broker