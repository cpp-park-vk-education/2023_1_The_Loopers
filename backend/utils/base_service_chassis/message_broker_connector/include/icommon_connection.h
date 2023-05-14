#pragma once

#include "inklink_global.h"

#include <memory>
#include <string>

namespace inklink::client_connector
{
class IClientSession;
}

namespace inklink::base_service_chassis
{
class ICommonConnection
{
    using IClientSession = client_connector::IClientSession;

public:
    virtual ~ICommonConnection() = default;

    virtual void Init(ServiceType, const Endpoint& self, const Endpoint& other) = 0;
    virtual void ChangeConnection(ServiceType, const Endpoint& self, const Endpoint& other) = 0;
    virtual std::shared_ptr<IClientSession> GetSession() = 0;

protected:
    std::shared_ptr<IClientSession> m_session;
    bool m_connected{false};
    ServiceType m_serviceType;
};
} // namespace inklink::base_service_chassis
