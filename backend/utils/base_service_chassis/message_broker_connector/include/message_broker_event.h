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
    MessageBrokerEvent(std::shared_ptr<ICommonConnection> cc,
                       std::function<void(const std::string&, error_code, IClientSession*)> readCallback,
                       std::shared_ptr<ILogger> logger);
    virtual ~MessageBrokerEvent() = default;

    virtual void SetDoOnNotified(std::function<void(int, const std::string&, IClientSession*)>) = 0;

    virtual void Publish(int event, const std::string&, ServiceType) = 0;
    virtual void Subscribe(int event, const Endpoint&) = 0;

protected:
    std::shared_ptr<ICommonConnection> m_connectionToMsgBroker;
    std::function<void(int, const std::string&, IClientSession*)> m_doOnNotified;
    std::shared_ptr<ILogger> m_logger;
};
} // namespace inklink::base_service_chassis
