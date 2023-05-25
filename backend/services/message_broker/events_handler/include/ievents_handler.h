#pragma once

#include "data_container.h"
#include "inklink_global.h"

#include <data_container.h>

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
    explicit IEventsHandler(IBaseServiceChassis&);
    virtual ~IEventsHandler() = default;
    virtual bool Handle(const serializer::DataContainer&, const Endpoint& sender);

protected:
    virtual void RemoveSubscriber(int eventType, const Endpoint&);
    virtual void AddSubscriber(int eventType, const Endpoint&);
    virtual void SendEvent(int eventType, const std::string&);

protected:
    IBaseServiceChassis& m_serviceChassis;

    std::unordered_map<int /*event type*/, std::vector<Endpoint>> m_subscribers;
};
} // namespace inklink::service_message_broker
