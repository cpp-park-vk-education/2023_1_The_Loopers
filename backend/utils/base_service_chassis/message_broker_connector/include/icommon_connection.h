#pragma once

#include "ilogger.h"
#include "inklink_global.h"

#include <global_websocket_client_session.h>

#include <boost/system/error_code.hpp>

#include <functional>
#include <memory>
#include <string>

namespace inklink::client_connector
{
class IClientSession;
}

namespace inklink::base_service_chassis
{
class ILogger;

class ICommonConnection
{
    using IClientSession = client_connector::IClientSession;
    using ConnectType = client_connector::ConnectType;
    using error_code = boost::system::error_code;

public:
    explicit ICommonConnection(std::shared_ptr<ILogger> logger);
    virtual ~ICommonConnection() = default;

    virtual void Init(ServiceType, const Endpoint& self, const Endpoint& other) = 0;
    virtual void ChangeConnection(ServiceType, const Endpoint& self, const Endpoint& other) = 0;
    [[nodiscard]] virtual std::shared_ptr<IClientSession> GetSession() const noexcept = 0;

    void SetAcceptCallback(std::function<void(ConnectType, error_code, IClientSession*)> callback) noexcept;
    void SetReadCallback(std::function<void(const std::string&, error_code, IClientSession*)> callback) noexcept;
    void SetWriteCallback(std::function<void(error_code)> callback) noexcept;

protected:
    std::weak_ptr<IClientSession> m_session;

    ServiceType m_serviceType;
    Endpoint m_endpointSelf;
    Endpoint m_endpointOther;
    bool m_connected{false};

    std::function<void(ConnectType, error_code, IClientSession*)> m_acceptCallback;
    std::function<void(const std::string&, error_code, IClientSession*)> m_readCallback;
    std::function<void(error_code)> m_writeCallback;

    std::shared_ptr<ILogger> m_logger;
};
} // namespace inklink::base_service_chassis
