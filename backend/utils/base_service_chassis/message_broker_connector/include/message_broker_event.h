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
public:
    using IClientSession = client_connector::IClientSession;
    using error_code = boost::system::error_code;

    using NotifiedFunctor = std::function<void(int /*event type*/, const std::string&, const Endpoint& /*from*/)>;

public:
    MessageBrokerEvent(std::shared_ptr<ICommonConnection>, NotifiedFunctor notifiedCallback, std::shared_ptr<ILogger>);
    virtual ~MessageBrokerEvent() = default;

    virtual void Publish(int event, const std::string&, ServiceType subscribersType = ServiceType::kAll);
    virtual void Subscribe(int event);

private:
    void DoOnNotified(const std::string&, error_code, IClientSession*) const;

private:
    std::shared_ptr<ICommonConnection> m_connectionToMsgBroker;
    NotifiedFunctor m_notifiedCallback;
    std::shared_ptr<ILogger> m_logger;
};
} // namespace inklink::base_service_chassis
