#pragma once

#include "ievents_handler.h"
#include "inklink_global.h"
#include "isignals_handler.h"

#include <internal_sessions_manager.h>

#include <boost/system/error_code.hpp>

#include <chrono>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace inklink::base_service_chassis
{
struct IBaseServiceChassis;
} // namespace inklink::base_service_chassis

namespace inklink::server_network
{
class IServiceSession;
} // namespace inklink::server_network

namespace inklink::service_message_broker
{
class MessageBrokerService
{
public:
    int Run();

private:
    using IBaseServiceChassis = base_service_chassis::IBaseServiceChassis;
    using IServiceSession = server_network::IServiceSession;

    using error_code = boost::system::error_code;

    using time_point = std::chrono::time_point<std::chrono::system_clock>;

    void DoOnRead(const std::string&, error_code, IServiceSession*);
    void DoOnConnect(error_code, IServiceSession*);
    void DoOnWrite(error_code, IServiceSession*);

    std::unique_ptr<IEventsHandler> m_eventsHandler;
    std::unique_ptr<ISignalsHandler> m_signalsHandler;

    // TODO (a.novak) from config file
    std::string m_address{"127.0.0.1"};
    std::uint16_t m_port{3996};

    std::unique_ptr<IBaseServiceChassis> m_chassis;
};
} // namespace inklink::service_message_broker
