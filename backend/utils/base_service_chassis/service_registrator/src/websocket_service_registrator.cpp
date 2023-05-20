#include "websocket_service_registrator.h"

#include "ilogger.h"

#include <global_websocket_client_session.h>
#include <websocket_client_session.h>

#include <boost/system/error_code.hpp>

#include <chrono>
#include <exception>
#include <stdexcept>
#include <thread>

namespace
{
using IClientSession = inklink::client_connector::IClientSession;
using error_code = boost::system::error_code;
using ConnectType = client_connector::ConnectType;
} // namespace

namespace inklink::base_service_chassis
{
WebsocketServiceRegistrator::WebsocketServiceRegistrator(std::shared_ptr<ILogger> logger)
        : IServiceRegistrator{}, m_logger{logger}
{
    // I could not find the way to deduce template params automatically
    auto lamOnAccept = [this](ConnectType type, error_code ec, IClientSession*)
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

    auto lamOnRead = [this](const std::string& str, error_code ec) { this->DoOnRead(str, ec); };

    auto session = std::make_shared<WebsocketClientSession<decltype(lamOnAccept), decltype(lamOnRead)>>(
            m_ioContext, lamOnAccept, lamOnRead);
    session->RunAsync(m_serviceRegistryAddress, m_serviceRegistryPort);
    m_connectionToRegistry = session;

    m_ioContextExecutor =
            boost::asio::require(m_ioContext.get_executor(), boost::asio::execution::outstanding_work.tracked);

    m_threadIoContext = std::thread([this]() { this->m_ioContext.run(); });
}

WebsocketServiceRegistrator::~WebsocketServiceRegistrator()
{
    m_ioContext.stop();
    m_threadIoContext.join();
}

bool WebsocketServiceRegistrator::Register(ServiceType type, const Endpoint& endpoint)
{
    auto session = InitSending("Session with service registry expired before registration!", true);

    // TODO (a.novak) serialize
    session->Send("protocol register message");

    while (!m_newMsg)
    {
        std::this_thread::sleep_for(100ms);
    }

    // TODO (a.novak) parse m_msg
    return true;
}

void WebsocketServiceRegistrator::Deregister(ServiceType type, const Endpoint& endpoint)
{
    auto session = InitSending("Session with service registry expired before registration!", true);
    // TODO (a.novak) serialize
    session->Send("protocol deregister message");

    while (!m_newMsg)
    {
        std::this_thread::sleep_for(100ms);
    }

    if (m_errCode) [[unlikely]]
    {
    }
    // TODO (a.novak) parse m_msg
}

[[nodiscard]] std::vector<Endpoint> WebsocketServiceRegistrator::GetEndpoints(ServiceType desiredServicesType)
{
    auto session = InitSending("Session with service registry expired. Can not get new endpoints.");

    if (!session) [[unlikely]]
    {
        throw std::runtime_error("Session with service registry expired.");
    }

    // TODO (a.novak) serialize
    session->Send("protocol get endpoints");

    while (!m_newMsg)
    {
        std::this_thread::sleep_for(10ms);
    }

    if (m_errCode) [[unlikely]]
    {
    }
    // TODO (a.novak) parse m_msg
}
void WebsocketServiceRegistrator::GetEndpoints(ServiceType desiredServicesType,
                                               std::function<void(std::vector<Endpoint>&&)> GotCallback)
{
    auto session = InitSending("Session with service registry expired. Can not get new endpoints.");

    if (!session) [[unlikely]]
    {
        throw std::runtime_error("Session with service registry expired.");
    }
    // TODO (a.novak) serialize
    session->Send("protocol get endpoints");

    auto waitForMsg = [&GotCallback, this]()
    {
        while (!m_newMsg)
        {
            std::this_thread::sleep_for(10ms);
        }

        if (m_errCode) [[unlikely]]
        {
        }
        // TODO (a.novak) parse m_msg
        // GotCallback(std::move(result));
        GotCallback({});
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
}

void WebsocketServiceRegistrator::DoOnRead(const std::string& str, error_code ec)
{
    m_newMsg = true;
    m_msg = str;
    m_errCode = ec;
}

} // namespace inklink::base_service_chassis
