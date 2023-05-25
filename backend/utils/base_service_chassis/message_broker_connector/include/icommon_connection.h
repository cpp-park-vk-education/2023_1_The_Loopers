#pragma once

#include "ilogger.h"
#include "inklink_global.h"

#include <websocket_fwd.h>

#include <boost/system/error_code.hpp>

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace inklink::client_connector
{
class IClientSession;
}

namespace inklink::base_service_chassis
{
class ILogger;

class ICommonConnection
{
public:
    using IClientSession = client_connector::IClientSession;
    using ConnectType = client_connector::ConnectType;
    using error_code = boost::system::error_code;

    using AcceptFunctor = std::function<void(ConnectType, error_code, IClientSession*)>;
    using ReadFunctor = std::function<void(const std::string&, error_code, IClientSession*)>;
    using WriteFunctor = std::function<void(error_code)>;

public:
    explicit ICommonConnection(std::shared_ptr<ILogger> logger);
    virtual ~ICommonConnection() = default;

    virtual void Init(ServiceType, const Endpoint& self, const Endpoint& other) = 0;
    virtual void ChangeConnection(ServiceType, const Endpoint& self, const Endpoint& other) = 0;
    [[nodiscard]] virtual std::shared_ptr<IClientSession> GetSession() const noexcept = 0;

    void AddAcceptCallback(AcceptFunctor callback);
    void AddReadCallback(ReadFunctor callback);
    void AddWriteCallback(WriteFunctor callback);

    [[nodiscard]] virtual Endpoint GetEndpointSelf() const noexcept;

protected:
    std::weak_ptr<IClientSession> m_session;

    ServiceType m_serviceType;
    Endpoint m_endpointSelf;
    Endpoint m_endpointOther;
    bool m_connected{false};

    std::vector<AcceptFunctor> m_acceptCallbacks;
    std::vector<ReadFunctor> m_readCallbacks;
    std::vector<WriteFunctor> m_writeCallbacks;

    std::shared_ptr<ILogger> m_logger;
};
} // namespace inklink::base_service_chassis
