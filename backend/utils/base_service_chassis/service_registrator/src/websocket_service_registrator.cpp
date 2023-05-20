#include "websocket_service_registrator.h"

#include "global_websocket_client_session.h"
#include "ilogger.h"

#include <websocket_client_session.h>

#include <boost/system/error_code.hpp>

namespace
{
using IClientSession = inklink::client_connector::IClientSession;
using error_code = boost::system::error_code;
using ConnectType = client_connector::ConnectType;
} // namespace

class YourClass
{
public:
    // remember, it will be called from different thread! (where running io_context)
    void DoOnAccept(error_code)
    {
        // your logic (log if (ec), for example)
        return;
    }

    void DoOnRead(ConnectType, error_code ec, IClientSession*)
    {
        // your logic
        return;
    }
};

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

    m_ioContextExecutor =
            boost::asio::require(m_ioContext.get_executor(), boost::asio::execution::outstanding_work::tracked);

    m_threadIoContext = std::thread([this]() { this->m_ioContext.run(); });
}

WebsocketServiceRegistrator::~WebsocketServiceRegistrator()
{
    m_ioContext.stop();
    m_threadIoContext.join();
}

bool WebsocketServiceRegistrator::Register(ServiceType, const Endpoint&)
{
}

void WebsocketServiceRegistrator::Deregister(ServiceType, const Endpoint&)
{
}

[[nodiscard]] std::vector<Endpoint> WebsocketServiceRegistrator::GetEndpoints(ServiceType desiredServicesType)
{
}
void WebsocketServiceRegistrator::GetEndpoints(ServiceType desiredServicesType,
                                               std::function<void(std::vector<Endpoint>&&)> GetCallback)
{
}

void WebsocketServiceRegistrator::DoOnRead(const std::string& str, error_code ec)
{
    // TODO (a.novak)
}

} // namespace inklink::base_service_chassis
