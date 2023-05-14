#pragma once

#include "global.h"
#include "iclient_session.h"

#include <memory>
#include <string>

namespace inklink::base_service_chassis
{
class ICommonConnection
{
public:
    virtual ~ICommonConnection() = default;

    virtual void Init(ServiceType, const Endpoint& self, const Endpoint& other) = 0;

    virtual void ChangeConnection(ServiceType, const Endpoint& self, const Endpoint& other) = 0;

    virtual std::shared_ptr<IClientSession> GetSession() = 0;

protected:
    std::shared_ptr<IClientSession> m_session;
    bool connected{false};
    ServiceType m_serviceType;
};
} // namespace inklink::base_service_chassis
