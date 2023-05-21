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
    explicit ISignalsHandler(std::unique_ptr<IBaseServiceChassis>&);
    virtual ~ISignalsHandler() = default;

    virtual void Send(const Endpoint&, const std::string&);

protected:
    std::unique_ptr<IBaseServiceChassis>& m_serviceChassis;
};
} // namespace inklink::service_message_broker
