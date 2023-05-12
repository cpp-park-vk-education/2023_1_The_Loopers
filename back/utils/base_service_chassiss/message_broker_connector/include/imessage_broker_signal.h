#ifndef _IMESSAGEBROKERSIGNAL_H_
#define _IMESSAGEBROKERSIGNAL_H_

#include <functional>
#include <memory>
#include <string>

#include "global.h"
#include "iclient_session.h"
#include "icommon_connection.h"

class IMessageBrokerSignal
{
public:
    virtual ~IMessageBrokerSignal() = default;

    virtual void SetCommonConnection(ICommonConnection*)
    {
    }

    virtual void SetDoOnRead(std::function<void(const std::string&, IClientSession*)> =
                                     [](const std::string&, IClientSession*) {})
    {
    }

    virtual void Request(const std::string&, const Endpoint&)
    {
    }

protected:
    std::shared_ptr<ICommonConnection> m_connectionToMsgBroker;
};

#endif  // _IMESSAGEBROKERSIGNAL_H_
