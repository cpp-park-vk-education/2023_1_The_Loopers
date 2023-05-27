#include "message_broker_service.h"

#include <data_container.h>
#include <iauthorizer.h>
#include <ibase_service_chassis.h>
#include <inklink/chassis_configurators/base_websocket_configurator.h>
#include <iservice_session.h>
#include <websocket_session_factory.h>

#include <json_serializer.h>

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

#include <chrono>
#include <filesystem>
#include <format>
#include <iostream>
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
constexpr const char* kLogPathPrefix = "inklink/message_broker/";

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
    auto authorizer = std::make_shared<IAuthorizer>();
    auto onReadFunctor = [this](const std::string& str, error_code ec, IServiceSession* iss)
    { DoOnRead(str, ec, iss); };
    auto onConnectFunctor = [this](error_code ec, IServiceSession* iss) { DoOnConnect(ec, iss); };
    auto onWriteFunctor = [this](error_code ec, IServiceSession* iss) { DoOnWrite(ec, iss); };

    auto factory = std::make_unique<server_network::WebsocketSessionsFactory<
            decltype(onReadFunctor), decltype(onConnectFunctor), decltype(onWriteFunctor)>>( // I think, it's ok with
                                                                                             // default template params
            manager, authorizer, onReadFunctor, onConnectFunctor, onWriteFunctor);

    std::filesystem::create_directories(kLogPathPrefix);
    // TODO (a.novak) add time to file name. For some reason, std format does not work
    //     const auto startTime = std::chrono::system_clock::now();
    const std::string logPath{std::string(kLogPathPrefix) + "message_broker_.txt"};
    //      + std::format("{:%Y_%m_%d_%H_%M}", startTime) + ".txt"};
    // clang-format off
    m_chassis = chassis_configurator::BaseChassisWebsocketConfigurator::CreateAndInitializeChassisWithoutMsgBroker(
            "message broker", logPath, 
            ioContext, std::move(factory), manager, 
            ServiceType::kMessageBroker, {.address = m_address, .port = m_port});
    // clang-format on
    m_chassis->logger->LogInfo("Message broker service is initted");

    std::cout << "Message broker configured" << __LINE__ << std::endl;

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
        m_chassis->logger->LogDebug(std::string("Got error while reading from '...'. Error: ")
#ifdef BOOST_OS_WINDOWS
                                            + ec.what();
#else
                                            ;
#endif
        );
        return;
    }
    const auto& msgData = JsonSerializer::ParseFromString(msg);
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
        m_chassis->logger->LogDebug(std::string("Got error from ... when tried to accept. Error: ")
#ifdef BOOST_OS_WINDOWS
                                    + ec.what()
#else

#endif
        );
        return;
    }
}

void MessageBrokerService::DoOnWrite(error_code ec, IServiceSession*)
{
    if (ec)
    {
        m_chassis->logger->LogDebug(std::string("Got error from ... while writing. Error: ")
#ifdef BOOST_OS_WINDOWS
                                    + ec.what()
#else

#endif
        );
        return;
    }
}
} // namespace inklink::service_message_broker
