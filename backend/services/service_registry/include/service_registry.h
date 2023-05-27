#pragma once

#include "ibase_service_chassis.h"
#include "inklink_global.h"

#include <data_container.h>

#include <chrono>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace inklink::server_network
{
class IServiceSession;
} // namespace inklink::server_network

namespace inklink::base_service_chassis
{
struct IBaseServiceChassis;
}

namespace inklink::service_registry
{
class ServiceRegistry
{
public:
    using DataContainer = serializer::DataContainer;

public:
    enum RegistryActionType
    {
        kRegister = 0,
        kExit,
        kGetServicesList
    };

public:
    int Run();

private:
    using IBaseServiceChassis = base_service_chassis::IBaseServiceChassis;
    using IServiceSession = server_network::IServiceSession;

    using error_code = boost::system::error_code;

    using time_point = std::chrono::time_point<std::chrono::system_clock>;

private:
    void DoOnRead(const std::string&, error_code, IServiceSession*);
    void DoOnConnect(error_code, IServiceSession*);
    void DoOnWrite(error_code, IServiceSession*);

    [[nodiscard]] DataContainer HandleRegisterQuery(const DataContainer&);
    [[nodiscard]] DataContainer HandleExitQuery(const DataContainer&);
    [[nodiscard]] DataContainer HandleGetServiceListQuery(const DataContainer&);

private:
    std::unordered_map<ServiceType, std::vector<Endpoint>> m_servicesAsServer;
    std::unordered_map<Endpoint, Endpoint> m_asClientFromServer;
    std::unordered_map<Endpoint, Endpoint> m_asServerFromClient;
    std::unordered_map<ServiceType, std::vector<Endpoint>> m_servicesAsClient;

    // TODO (a.novak) from config file
    std::string m_address{"127.0.0.1"};
    std::uint16_t m_port{3998};

    std::shared_ptr<IBaseServiceChassis> m_chassis;
};
} // namespace inklink::service_registry
