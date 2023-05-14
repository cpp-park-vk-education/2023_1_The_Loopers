#pragma once

#include "global.h"
#include "iclient_session.h"
#include "icommon_connection.h"

#include <functional>
#include <memory>
#include <string>

namespace inklink::base_service_chassis
{
class IMessageBrokerSignal
{
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
