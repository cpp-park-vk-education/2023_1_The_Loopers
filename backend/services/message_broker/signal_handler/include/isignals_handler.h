#pragma once

#include "inklink_global.h"

#include <data_container.h>
#include <ibase_service_chassis.h>

#include <memory>
#include <unordered_map>
#include <vector>

namespace inklink::base_service_chassis
{
struct IBaseServiceChassis;
}

namespace inklink::service_message_broker
{
class ISignalsHandler
{
public:
    using IBaseServiceChassis = base_service_chassis::IBaseServiceChassis;
    using DataContainer = serializer::DataContainer;

public:
    explicit ISignalsHandler(IBaseServiceChassis&);
    virtual ~ISignalsHandler() = default;

    bool Handle(const DataContainer&, const Endpoint& sender);

protected:
    virtual void Send(const Endpoint&, const std::string&);

protected:
    IBaseServiceChassis& m_serviceChassis;
};
} // namespace inklink::service_message_broker
