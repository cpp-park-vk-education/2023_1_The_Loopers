#pragma once

#include "inklink_global.h"

#include <cstdint>
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
class IServiceRegistrator
{
public:
    using IClientSession = client_connector::IClientSession;

    enum class ServiceRole
    {
        kServer,
        kClient
    };

public:
    virtual ~IServiceRegistrator() = default;

    // can discard
    virtual bool Register(ServiceType, const Endpoint& endpointAsServer, const Endpoint& endpointAsClient) = 0;
    virtual void Deregister(ServiceType, const Endpoint& endpointAsServer) = 0;

    /**
     * @brief Get the Endpoints synchronously
     *
     * @return std::vector<Endpoint>
     */
    [[nodiscard]] virtual std::vector<Endpoint> GetEndpoints(ServiceType desiredServicesType,
                                                             ServiceRole desiredRole) = 0;
    /**
     * @brief Get the Endpoints object asynchronously (will be passed to callback)
     *
     * @warning Callback may be called in different thread!
     */
    virtual void GetEndpoints(ServiceType desiredServicesType, ServiceRole desiredRole,
                              std::function<void(std::vector<Endpoint>&&)> GotCallback) = 0;

protected:
    // TODO (a.novak) move to config file
    std::string m_serviceRegistryAddress{"127.0.0.1"};
    std::uint16_t m_serviceRegistryPort{3998}; // free according to wikipedia.org/wiki/List_of_TCP_and_UDP_port_numbers

    std::weak_ptr<IClientSession> m_connectionToRegistry;
};
} // namespace inklink::base_service_chassis
