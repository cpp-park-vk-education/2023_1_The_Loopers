#include "websocket_service_registrator.h"

#include "ilogger.h"
#include "inklink_global.h"

#include <data_container.h>
#include <websocket_client_session.h>

#include <json_serializer.h>

#include <boost/system/error_code.hpp>

#include <chrono>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <thread>

namespace
{
using namespace std::chrono_literals;
using IClientSession = inklink::client_connector::IClientSession;
using error_code = boost::system::error_code;
using ConnectType = inklink::client_connector::ConnectType;

using JsonSerializer = inklink::serializer::JsonSerializer;
using DataContainer = inklink::serializer::DataContainer;
} // namespace

namespace inklink::base_service_chassis
{
WebsocketServiceRegistrator::WebsocketServiceRegistrator(std::shared_ptr<ILogger> logger)
        : IServiceRegistrator{}, m_logger{logger}
{
    // I could not find the way to deduce template params automatically
    auto onAcceptFunctor = [this](ConnectType type, error_code ec, IClientSession*)
    {
        if (ec)
        {
            m_logger->LogCritical("Error occurred when tried to connect to service registry! " + ec.what());
            throw ec; // app will crash because it will be thrown in thread without catch:
                      // ideally should catch it and rethrow in main thread, but, if I understand correctly, for this
                      // one need threads/ event manager like qt one and I will not do it
        }
        switch (type)
        {
        case ConnectType::kResolve:
            m_logger->LogDebug("Resolved service registry address");
            break;
        case ConnectType::kConnect:
            m_logger->LogInfo("Connected to service registry");
            break;
        case ConnectType::kHandshake:
            m_logger->LogDebug("Successful handshake with service registry");
            break;
        }
    };

    auto onReadFunctor = [this](const std::string& msg, error_code ec, IClientSession*) { this->DoOnRead(msg, ec); };

    std::cout << "Before create session!" << __LINE__ << std::endl;

    auto session = std::make_shared<
            client_connector::WebsocketClientSession<decltype(onAcceptFunctor), decltype(onReadFunctor)>>(
            m_ioContext, onAcceptFunctor, onReadFunctor);
    session->RunAsync(m_serviceRegistryAddress, m_serviceRegistryPort);
    m_connectionToRegistry = session;

    std::cout << "Session created!" << __LINE__ << std::endl;

    m_ioContextExecutor =
            boost::asio::require(m_ioContext.get_executor(), boost::asio::execution::outstanding_work.tracked);
    try
    {
        // this->m_ioContext.run();
        m_threadIoContext = std::thread([this]() { this->m_ioContext.run(); });
    }
    catch (...)
    {
        std::cout << "Error accured on run!" << __LINE__ << std::endl;
    }
}

WebsocketServiceRegistrator::~WebsocketServiceRegistrator()
{
    m_ioContext.stop();
    m_threadIoContext.join();
}

bool WebsocketServiceRegistrator::Register(ServiceType type, const Endpoint& endpointAsServer,
                                           const Endpoint& endpointAsClient)
{
    auto session = InitSending("Session with service registry expired before registration!", true);

    DataContainer registerMsg{};
    registerMsg["action_type"] = 0; // kRegister
    registerMsg["description"]["service_type"] = static_cast<int>(type);
    registerMsg["description"]["endpoint_as_server"]["address"] = endpointAsServer.address;
    registerMsg["description"]["endpoint_as_server"]["port"] = static_cast<int>(endpointAsServer.port);
    registerMsg["description"]["endpoint_as_client"]["address"] = endpointAsClient.address;
    registerMsg["description"]["endpoint_as_client"]["port"] = static_cast<int>(endpointAsClient.port);
    session->Send(JsonSerializer::SerializeAsString(registerMsg));

    int counter{0};
    while (!m_newMsg)
    {
        ++counter;
        if (counter > 100)
        {
            std::cout << "10s more in a loop" << std::endl;
            counter = 0;
        }
        std::this_thread::sleep_for(100ms);
    }

    auto response = JsonSerializer::ParseFromString(m_msg);
    return response.AsInt("action_result") == 1;
}

void WebsocketServiceRegistrator::Deregister(ServiceType type, const Endpoint& endpointAsServer)
{
    auto session = InitSending("Session with service registry expired before registration!", true);

    DataContainer deregisterMsg{};
    deregisterMsg["action_type"] = 1; // kExit
    deregisterMsg["description"]["service_type"] = static_cast<int>(type);
    deregisterMsg["description"]["endpoint_as_server"]["address"] = endpointAsServer.address;
    deregisterMsg["description"]["endpoint_as_server"]["port"] = static_cast<int>(endpointAsServer.port);
    session->Send(JsonSerializer::SerializeAsString(deregisterMsg));

    while (!m_newMsg)
    {
        std::this_thread::sleep_for(100ms);
    }

    if (m_errCode) [[unlikely]]
    {
    }
    auto response = JsonSerializer::ParseFromString(m_msg);
    return;
}

[[nodiscard]] std::vector<Endpoint> WebsocketServiceRegistrator::GetEndpoints(ServiceType desiredServicesType,
                                                                              ServiceRole desiredRole)
{
    auto session = InitSending("Session with service registry expired. Can not get new endpoints.");

    if (!session) [[unlikely]]
    {
        throw std::runtime_error("Session with service registry expired.");
    }

    DataContainer getEndpointsMsg{};
    getEndpointsMsg["action_type"] = 2; // kGetEndpoints
    getEndpointsMsg["description"]["service_type"] = static_cast<int>(desiredServicesType);
    switch (desiredRole)
    {
    case ServiceRole::kServer:
        getEndpointsMsg["description"]["role"] = std::string("server");
        break;
    case ServiceRole::kClient:
        getEndpointsMsg["description"]["role"] = std::string("client");
        break;
    }
    session->Send(JsonSerializer::SerializeAsString(getEndpointsMsg));

    while (!m_newMsg)
    {
        std::this_thread::sleep_for(10ms);
    }

    if (m_errCode) [[unlikely]]
    {
    }

    std::vector<Endpoint> result;
    auto gotEndpoints = JsonSerializer::ParseFromString(m_msg);
    for (const auto& serviceList : gotEndpoints.AsArray("services"))
    {
        for (const auto& service : serviceList.AsArray("service_list"))
        {
            result.push_back(Endpoint{.address = service.AsString("address"),
                                      .port = static_cast<std::uint16_t>(service.AsInt("port"))});
        }
    }
    return result;
}
void WebsocketServiceRegistrator::GetEndpoints(ServiceType desiredServicesType, ServiceRole desiredRole,
                                               GotEndpointsCallback GotCallback)
{
    auto session = InitSending("Session with service registry expired. Can not get new endpoints.");

    if (!session) [[unlikely]]
    {
        throw std::runtime_error("Session with service registry expired.");
    }

    DataContainer getEndpointsMsg{};
    getEndpointsMsg["action_type"] = 2; // kGetEndpoints
    getEndpointsMsg["description"]["service_type"] = static_cast<int>(desiredServicesType);
    switch (desiredRole)
    {
    case ServiceRole::kServer:
        getEndpointsMsg["description"]["role"] = std::string("server");
        break;
    case ServiceRole::kClient:
        getEndpointsMsg["description"]["role"] = std::string("client");
        break;
    }
    session->Send(JsonSerializer::SerializeAsString(getEndpointsMsg));

    auto waitForMsg = [GotCallback, this]()
    {
        while (!m_newMsg)
        {
            std::this_thread::sleep_for(10ms);
        }

        if (m_errCode) [[unlikely]]
        {
        }

        std::vector<Endpoint> result;
        auto gotEndpoints = JsonSerializer::ParseFromString(m_msg);
        for (const auto& serviceList : gotEndpoints.AsArray("services"))
        {
            for (const auto& service : serviceList.AsArray("service_list"))
            {
                result.push_back(Endpoint{.address = service.AsString("address"),
                                          .port = static_cast<std::uint16_t>(service.AsInt("port"))});
            }
        }
        GotCallback(std::move(result));
    };

    std::thread gotThread{waitForMsg};
    gotThread.detach();
}

std::shared_ptr<IClientSession> WebsocketServiceRegistrator::InitSending(const std::string& errorMsg, bool critical)
{
    auto session = m_connectionToRegistry.lock();

    if (!session) [[unlikely]]
    {
        if (!critical)
        {
            m_logger->LogWarning(errorMsg);
        }
        else
        {
            m_logger->LogCritical(errorMsg);
            throw std::runtime_error(errorMsg);
        }
    }

    m_newMsg = false;
    m_msg.clear();

    return session;
}

void WebsocketServiceRegistrator::DoOnRead(const std::string& msg, error_code ec)
{
    m_newMsg = true;
    m_msg = msg;
    m_errCode = ec;
}

} // namespace inklink::base_service_chassis
