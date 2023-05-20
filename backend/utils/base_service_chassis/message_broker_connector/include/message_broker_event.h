#pragma once

#include "icommon_connection.h"
#include "inklink_global.h"

#include <boost/system/error_code.hpp>

#include <functional>
#include <memory>
#include <string>

namespace inklink::client_connector
{
class IClientSession;
}

namespace inklink::base_service_chassis
{
class ILogger;
class MessageBrokerEvent
{
    using IClientSession = client_connector::IClientSession;
    using error_code = boost::system::error_code;

public:
    MessageBrokerEvent(std::shared_ptr<ICommonConnection>,
                       std::function<void(int /*event type*/, const std::string&, Endpoint /*from*/)> notifiedCallback,
                       std::shared_ptr<ILogger>);
    virtual ~MessageBrokerEvent() = default;

    virtual void Publish(int event, const std::string&, ServiceType subscribersType = ServiceType::kAll);
    virtual void Subscribe(int event);

private:
    void DoOnNotified(const std::string&, error_code, IClientSession*) const;

    std::shared_ptr<ICommonConnection> m_connectionToMsgBroker;
    std::function<void(int, const std::string&, Endpoint)> m_notifiedCallback;
    std::shared_ptr<ILogger> m_logger;
};
} // namespace inklink::base_service_chassis
