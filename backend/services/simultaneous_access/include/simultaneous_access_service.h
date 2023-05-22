#pragma once

#include "idraw_conflict_resolver.h"
#include "inklink_global.h"
#include "itext_conflict_resolver.h"

#include <internal_sessions_manager.h>

#include <boost/system/error_code.hpp>

#include <chrono>
#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>

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
public:
    int Run();

private:
    using DocSessionDescriptor = base_service_chassis::DocSessionDescriptor;

    using IDBAdapter = db_adapter::IDBAdapter;
    using IExternalServiceChassis = external_service_chassis::IExternalServiceChassis;
    using IServiceSession = server_network::IServiceSession;

    using error_code = boost::system::error_code;

    using time_point = std::chrono::time_point<std::chrono::system_clock>;

    void DoOnRead(const std::string&, error_code, IServiceSession*);
    void DoOnConnect(error_code, IServiceSession*);
    void DoOnWrite(error_code, IServiceSession*);

    void DoOnNotified(int eventType, const std::string&, Endpoint from);
    void DoOnSignal(const std::string&);

    DrawAction ParseDrawAction(const std::string&, const Endpoint&);
    DrawAction ParseTextAction(const std::string&, const Endpoint&);

    std::unordered_map<DocSessionDescriptor, std::map<time_point, DrawAction>> m_drawNotApplied;
    std::unordered_map<DocSessionDescriptor, std::map<time_point, TextAction>> m_textNotApplied;

    // TODO (a.novak) from config file
    std::string m_address{"127.0.0.1"};
    std::uint16_t m_port{3997};

    std::unique_ptr<IExternalServiceChassis> m_chassis;
    std::unique_ptr<IDBAdapter> m_dbAdapter;
};
} // namespace inklink::service_simultaneous_access
