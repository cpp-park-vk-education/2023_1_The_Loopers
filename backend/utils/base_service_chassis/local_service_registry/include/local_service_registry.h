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

    [[nodiscard]] virtual const std::vector<Endpoint>& GetServices(ServiceType) const;
    [[nodiscard]] virtual ServiceType GetServiceType(const Endpoint&) const;
    [[nodiscard]] virtual bool HasEndpoint(const Endpoint&) const noexcept;

private:
    mutable std::unordered_map<ServiceType, std::vector<Endpoint>> m_endpointsByServiceType;
    std::unordered_map<Endpoint, ServiceType> m_serviceTypeByEndpoint;
};
} // namespace inklink::base_service_chassis
