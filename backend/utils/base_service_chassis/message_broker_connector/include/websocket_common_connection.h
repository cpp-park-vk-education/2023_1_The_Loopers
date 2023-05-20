#pragma once

#include "icommon_connection.h"

#include <boost/asio.hpp>

#include <memory>
#include <thread>

namespace
{
using IClientSession = client_connector::IClientSession;
using ConnectType = client_connector::ConnectType;
using error_code = boost::system::error_code;
} // namespace

namespace inklink::base_service_chassis
{
class WebsocketCommonConnection final : public ICommonConnection
{
    using IClientSession = client_connector::IClientSession;

public:
    explicit WebsocketCommonConnection(std::shared_ptr<ILogger>);

    ~WebsocketCommonConnection() final = default;

    void Init(ServiceType, const Endpoint& self, const Endpoint& other) final;
    void ChangeConnection(ServiceType, const Endpoint& self, const Endpoint& other) final;
    [[nodiscard]] std::shared_ptr<IClientSession> GetSession() const noexcept final;

private:
    boost::asio::io_context m_ioContext;
    std::thread m_threadIoContext;
    boost::asio::any_io_executor m_ioContextExecutor;
};
} // namespace inklink::base_service_chassis
