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
    virtual ~IMessageBrokerSignal() = default;

    virtual void SetCommonConnection(ICommonConnection*) = 0;

    virtual void SetDoOnRead(std::function<void(const std::string&, IClientSession*)> = [](const std::string&,
                                                                                           IClientSession*) {}) = 0;

    virtual void Request(const std::string&, const Endpoint&) = 0;

protected:
    std::shared_ptr<ICommonConnection> m_connectionToMsgBroker;
};
} // namespace inklink::base_service_chassis
