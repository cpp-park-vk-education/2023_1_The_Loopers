#include "simultaneous_access_service.h"

#include <iexternal_service_chassis.h>

namespace inklink::service_simultaneous_access
{
SimultaneousAccessService::SimultaneousAccessService(IResolversFactory& factory, IExternalServiceChassis& chassis,
                                                     IDBAdapter& dbAdapter) noexcept
        : m_factory{factory}, m_chassis{chassis}, m_dbAdapter{dbAdapter}
{
}

void SimultaneousAccessService::Run(uint16_t port)
{
}

void SimultaneousAccessService::DoOnConnect()
{
}
void SimultaneousAccessService::DoOnRead(const std::string&, IServiceSession*)
{
}
void SimultaneousAccessService::DoOnWrite()
{
}

} // namespace inklink::service_simultaneous_access
