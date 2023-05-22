#include "local_service_registry.h"

#include <exception>
#include <stdexcept>
#include <string>
#include <vector>

namespace inklink::base_service_chassis
{

bool LocalServiceRegistry::AddService(ServiceType serviceType, const Endpoint& endpoint)
{
    if (HasEndpoint(endpoint))
    {
        return false;
    }

    m_serviceMap[serviceType].push_back(endpoint);
    m_endpointMap[endpoint] = serviceType;
    return true;
}

bool LocalServiceRegistry::RemoveService(const Endpoint& endpoint)
{
    if (!HasEndpoint(endpoint))
    {
        return false;
    }

    auto type = m_endpointMap[endpoint];
    std::erase(m_serviceMap[type], endpoint);
    m_endpointMap.erase(endpoint);
    return true;
}

std::vector<Endpoint> LocalServiceRegistry::GetServices(ServiceType serviceType) const noexcept
{
    auto it = m_serviceMap.find(serviceType);
    if (it != m_serviceMap.end())
    {
        return it->second;
    }
    return {};
}

ServiceType LocalServiceRegistry::GetServiceType(const Endpoint& endpoint) const
{
    auto it = m_endpointMap.find(endpoint);
    if (it != m_endpointMap.end())
    {
        return it->second;
    }
    // TODO (a.novak) after add to_string()/ smth like this to Endpoint, add info about it
    throw std::out_of_range("There is no endpoint : '' in local service registry");
}

bool LocalServiceRegistry::HasEndpoint(const Endpoint& endpoint) const noexcept
{
    auto has{false};
    auto it = m_endpointMap.find(endpoint);
    if (it != m_endpointMap.end())
    {
        has = true;
    }
    return has;
}
} // namespace inklink::base_service_chassis
