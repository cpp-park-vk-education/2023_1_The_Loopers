#pragma once

#include "ibase_service_chassis.h"
#include "inklink_global.h"

#include <memory>
#include <unordered_map>
#include <vector>

namespace inklink::base_service_chassis
{
class IBaseServiceChassis;
}

namespace inklink::service_message_broker
{
class ISignalsHandler
{
    using IBaseServiceChassis = base_service_chassis::IBaseServiceChassis;

public:
    ISignalsHandler(std::shared_ptr<IBaseServiceChassis>);

    virtual ~ISignalsHandler() = default;

    virtual void Send(Endpoint, const std::string&) = 0;

protected:
    std::shared_ptr<IBaseServiceChassis> m_serviceChassis;
};
} // namespace inklink::service_message_broker
