#pragma once

#include "icommon_connection.h"
#include "inklink_global.h"

#include <functional>
#include <memory>
#include <string>

namespace inklink::client_connector
{
class IClientSession;
}

namespace inklink::base_service_chassis
{
class IMessageBrokerSignal
{
    using IClientSession = client_connector::IClientSession;

public:
    explicit IMessageBrokerSignal(std::shared_ptr<ICommonConnection> cc) noexcept
            : m_connectionToMsgBroker{std::move(cc)}
    {
    }
    virtual ~IMessageBrokerSignal() = default;

    virtual void SetDoOnRead(std::function<void(const std::string&, IClientSession*)>) = 0;
    virtual void Request(const std::string&, const Endpoint&) = 0;

protected:
    std::shared_ptr<ICommonConnection> m_connectionToMsgBroker;
    std::function<void(const std::string&, IClientSession*)> m_doOnRead;
};
} // namespace inklink::base_service_chassis
