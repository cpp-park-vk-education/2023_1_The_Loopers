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
    IEventsHandler(std::shared_ptr<IBaseServiceChassis>);

    virtual ~IEventsHandler() = default;

    virtual void RemoveSubscriber(int, const Endpoint&) = 0;
    virtual void AddSubscriber(int, const Endpoint&) = 0;
    virtual void SendEvent(int, const std::string&) = 0;

protected:
    std::shared_ptr<IBaseServiceChassis> m_serviceChassis;

    std::unordered_map<int, std::vector<Endpoint>> m_subscribers;
};
} // namespace inklink::service_message_broker
