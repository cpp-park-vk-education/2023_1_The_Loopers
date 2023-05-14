#pragma once

#include "global.h"
#include "ibase_service_chassiss.h"

#include <memory>
#include <unordered_map>
#include <vector>

namespace inklink::service_message_broker
{
class IEventsHandler
{
public:
    virtual ~IEventsHandler() = default;

    virtual void SetServiceChassis(std::shared_ptr<IBaseServiceChassis>) = 0;

    virtual void RemoveSubscriber(int, Endpoint) = 0;
    virtual void AddSubscriber(int, Endpoint) = 0;
    virtual void SendEvent(int, std::string) = 0;

protected:
    std::shared_ptr<IBaseServiceChassis> m_serviceChassis{std::make_shared<IBaseServiceChassis>()};

    std::unordered_map<int, std::vector<Endpoint>> m_subscribers;
};
} // namespace inklink::service_message_broker
