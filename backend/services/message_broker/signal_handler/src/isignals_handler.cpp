#include "isignals_handler.h"

#include <ibase_service_chassis.h>

namespace
{
using IBaseServiceChassis = inklink::base_service_chassis::IBaseServiceChassis;
} // namespace

namespace inklink::service_message_broker
{
ISignalsHandler::ISignalsHandler(std::unique_ptr<IBaseServiceChassis>& chassis) : m_serviceChassis{chassis}
{
}

void ISignalsHandler::Send(const Endpoint&, const std::string&)
{
    // TODO
}

} // namespace inklink::service_message_broker
