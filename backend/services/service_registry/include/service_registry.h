#pragma once

#include "inklink_global.h"

#include <iexternal_service_chassis.h>

#include <chrono>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace inklink::server_network
{
class IServiceSession;
} // namespace inklink::server_network

namespace inklink::service_registry
{
class ServiceRegistry
{
public:
    int Run();

private:
    using IExternalServiceChassis = external_service_chassis::IExternalServiceChassis;
    using IServiceSession = server_network::IServiceSession;

    using error_code = boost::system::error_code;

    using time_point = std::chrono::time_point<std::chrono::system_clock>;

    void DoOnRead(const std::string&, error_code, IServiceSession*);
    void DoOnConnect(error_code, IServiceSession*);
    void DoOnWrite(error_code, IServiceSession*);

    std::unordered_map<ServiceType, std::vector<Endpoint>> m_services;

    // TODO (a.novak) from config file
    std::string m_address{"127.0.0.1"};
    std::uint16_t m_port{3998};

    std::unique_ptr<IExternalServiceChassis> m_chassis;
};
} // namespace inklink::service_registry
