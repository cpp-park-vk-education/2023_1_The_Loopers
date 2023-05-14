#pragma once

#include "iexternal_service_chassis.h"
#include "iresolvers_factory.h"

#include <map>
#include <memory>
#include <string>

namespace inklink::db_adapter
{
class IDBAdapter;
}

namespace inklink::external_service_chassis
{
class IExternalServiceChassis;
}

namespace inklink::server_network
{
class IServiceSession;
}

namespace inklink::service_simultaneous_access
{
class SimultaneousAccessService
{
    using IDBAdapter = db_adapter::IDBAdapter;
    using IExternalServiceChassis = external_service_chassis::IExternalServiceChassis;
    using IServiceSession = server_network::IServiceSession;

public:
    SimultaneousAccessService(IResolversFactory&, IExternalServiceChassis&, IDBAdapter&);

    void Run(unsigned short port);

private:
    void DoOnConnect();
    void DoOnRead(const string&, IServiceSession*);
    void DoOnWrite();

    std::map<std::chrono::time_point<std::chrono::system_clock>, DrawAction> m_notApplied;

    IResolversFactory& m_factory;
    IExternalServiceChassis& m_chassis;
    IDBAdapter& m_dbAdapter;
};
} // namespace inklink::service_simultaneous_access
