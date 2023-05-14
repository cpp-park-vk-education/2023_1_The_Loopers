#pragma once

#include "global.h"

#include <memory>
#include <string>
#include <vector>

class IClientSession;

namespace inklink::base_service_chassis
{
class IServiceRegistrator
{
public:
    virtual ~IServiceRegistrator() = default;

    virtual void SetConnection(IClientSession*) = 0;

    virtual bool Register(ServiceType, const Endpoint&) = 0;
    virtual void Deregister(ServiceType, const Endpoint&) = 0;
    virtual std::vector<Endpoint> GetEndpoints(ServiceType) = 0;

protected:
    std::shared_ptr<IClientSession> m_cnnectionToRegistry;
};
} // namespace inklink::base_service_chassis
