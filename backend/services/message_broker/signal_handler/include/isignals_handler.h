#pragma once

#include "global.h"
#include "ibase_service_chassiss.h"

#include <memory>
#include <unordered_map>
#include <vector>

namespace inklink::service_message_broker
{
class ISignalsHandler
{
public:
    virtual ~ISignalsHandler() = default;

    virtual void SetServiceChassis(std::shared_ptr<IBaseServiceChassis>) = 0;

    virtual void Send(Endpoint, const std::string&) = 0;

protected:
    std::shared_ptr<IBaseServiceChassis> m_serviceChassis;
};
} // namespace inklink::service_message_broker
