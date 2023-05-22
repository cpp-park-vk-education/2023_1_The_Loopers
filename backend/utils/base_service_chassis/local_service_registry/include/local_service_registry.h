#pragma once

#include "inklink_global.h"

#include <string>
#include <unordered_map>
#include <vector>

namespace inklink::base_service_chassis
{
class LocalServiceRegistry
{
public:
    virtual ~LocalServiceRegistry() = default;

    virtual bool AddService(ServiceType, const Endpoint&);
    virtual bool RemoveService(const Endpoint&);

    [[nodiscard]] virtual std::vector<Endpoint> GetServices(ServiceType) const noexcept;
    [[nodiscard]] virtual ServiceType GetServiceType(const Endpoint&) const;
    [[nodiscard]] virtual bool HasEndpoint(const Endpoint&) const noexcept;

private:
    std::unordered_map<ServiceType, std::vector<Endpoint>> m_serviceMap;
    std::unordered_map<Endpoint, ServiceType> m_endpointMap;
};
} // namespace inklink::base_service_chassis
