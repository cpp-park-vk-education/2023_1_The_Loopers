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
public:
    using IClientSession = client_connector::IClientSession;
    using error_code = boost::system::error_code;
    using GotEndpointsCallback = std::function<void(std::vector<Endpoint>&&)>;

public:
    explicit WebsocketServiceRegistrator(std::shared_ptr<ILogger>);
    ~WebsocketServiceRegistrator() override;

    // can discard
    bool Register(ServiceType, const Endpoint&) override;
    void Deregister(ServiceType, const Endpoint&) override;

    [[nodiscard]] std::vector<Endpoint> GetEndpoints(ServiceType desiredServicesType) override;
    void GetEndpoints(ServiceType desiredServicesType, GotEndpointsCallback GotCallback) override;

private:
    [[nodiscard]] std::shared_ptr<IClientSession> InitSending(const std::string& errorMsg, bool critical = false);
    void DoOnRead(const std::string& str, error_code ec);

private:
    bool m_newMsg{false};
    std::string m_msg;
    error_code m_errCode;

    boost::asio::io_context m_ioContext;
    std::thread m_threadIoContext;
    boost::asio::any_io_executor m_ioContextExecutor;

    std::shared_ptr<ILogger> m_logger;
};
} // namespace inklink::base_service_chassis
