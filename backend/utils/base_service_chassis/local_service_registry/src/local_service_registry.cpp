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

    m_endpointsByServiceType[serviceType].push_back(endpoint);
    m_serviceTypeByEndpoint[endpoint] = serviceType;
    return true;
}

bool LocalServiceRegistry::RemoveService(const Endpoint& endpoint)
{
    if (!HasEndpoint(endpoint))
    {
        return false;
    }

    auto type = m_serviceTypeByEndpoint[endpoint];
    std::erase(m_endpointsByServiceType[type], endpoint);
    m_serviceTypeByEndpoint.erase(endpoint);
    return true;
}

const std::vector<Endpoint>& LocalServiceRegistry::GetServices(ServiceType serviceType) const
{
    return m_endpointsByServiceType[serviceType];
}

ServiceType LocalServiceRegistry::GetServiceType(const Endpoint& endpoint) const
{
    auto it = m_serviceTypeByEndpoint.find(endpoint);
    if (it != m_serviceTypeByEndpoint.end())
    {
        return it->second;
    }
    // TODO (a.novak) after add to_string()/ smth like this to Endpoint, add info about it
    throw std::out_of_range("There is no endpoint : '' in local service registry");
}

bool LocalServiceRegistry::HasEndpoint(const Endpoint& endpoint) const noexcept
{
    auto has{false};
    auto it = m_serviceTypeByEndpoint.find(endpoint);
    if (it != m_serviceTypeByEndpoint.end())
    {
        has = true;
    }
    return has;
}
} // namespace inklink::base_service_chassis
