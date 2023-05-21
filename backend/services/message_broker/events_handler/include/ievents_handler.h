#pragma once

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
class IEventsHandler
{
    using IBaseServiceChassis = base_service_chassis::IBaseServiceChassis;

public:
    explicit IEventsHandler(std::unique_ptr<IBaseServiceChassis>&);

    virtual ~IEventsHandler() = default;

    virtual void RemoveSubscriber(int, const Endpoint&);
    virtual void AddSubscriber(int, const Endpoint&);
    virtual void SendEvent(int, const std::string&);

protected:
    std::unique_ptr<IBaseServiceChassis>& m_serviceChassis;

    std::unordered_map<int /*event type*/, std::vector<Endpoint>> m_subscribers;
};
} // namespace inklink::service_message_broker
