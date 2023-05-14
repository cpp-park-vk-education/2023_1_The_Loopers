#pragma once

#include "global.h"
#include "iclient_session.h"
#include "icommon_connection.h"

#include <functional>
#include <memory>
#include <string>

namespace inklink::base_service_chassis
{
class IMessageBrokerEvent
{
public:
    virtual ~IMessageBrokerEvent() = default;

    virtual void SetCommonConnection(ICommonConnection*) = 0;

    virtual void SetDoOnNotified(std::function<void(int, const std::string&, IClientSession*)> =
                                         [](int, const std::string&, IClientSession*) {}) = 0;

    virtual void Publish(int event, const std::string&, ServiceType = ServiceType::kNone) = 0;

    virtual void Subscribe(int event, const Endpoint&) = 0;

protected:
    std::shared_ptr<ICommonConnection> m_connectionToMsgBroker;
    std::function<void(int, const std::string&, IClientSession*)> m_doOnNotified;
};
} // namespace inklink::base_service_chassis
