#pragma once

#include "inklink_global.h"

#include <memory>
#include <string>
#include <vector>

namespace inklink::client_connector
{
class IClientSession;
}

namespace inklink::base_service_chassis
{
class IServiceRegistrator
{
    using IClientSession = client_connector::IClientSession;

public:
    virtual ~IServiceRegistrator() = default;

    virtual void SetConnection(IClientSession*) = 0;

    virtual bool Register(ServiceType, const Endpoint&) = 0;
    virtual void Deregister(ServiceType, const Endpoint&) = 0;
    virtual std::vector<Endpoint> GetEndpoints(ServiceType) = 0;

protected:
    std::shared_ptr<IClientSession> m_connectionToRegistry;
};
} // namespace inklink::base_service_chassis
