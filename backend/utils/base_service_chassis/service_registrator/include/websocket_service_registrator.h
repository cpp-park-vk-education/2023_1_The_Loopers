#pragma once

#include "iservice_registrator.h"

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

#include <memory>
#include <thread>

namespace inklink::base_service_chassis
{
class ILogger;

class WebsocketServiceRegistrator final : public IServiceRegistrator
{
    using IClientSession = client_connector::IClientSession;
    using error_code = boost::system::error_code;

public:
    explicit WebsocketServiceRegistrator(std::shared_ptr<ILogger>);
    ~WebsocketServiceRegistrator() final;

    // can discard
    bool Register(ServiceType, const Endpoint&) final;
    void Deregister(ServiceType, const Endpoint&) final;

    [[nodiscard]] std::vector<Endpoint> GetEndpoints(ServiceType desiredServicesType) final;
    void GetEndpoints(ServiceType desiredServicesType, std::function<void(std::vector<Endpoint>&&)> GotCallback) final;

private:
    [[nodiscard]] std::shared_ptr<IClientSession> InitSending(const std::string& errorMsg, bool critical = false);
    void DoOnRead(const std::string& str, error_code ec);

    bool m_newMsg{false};
    std::string m_msg;
    error_code m_errCode;

    boost::asio::io_context m_ioContext;
    std::thread m_threadIoContext;
    boost::asio::any_io_executor m_ioContextExecutor;

    std::shared_ptr<ILogger> m_logger;
};
} // namespace inklink::base_service_chassis
