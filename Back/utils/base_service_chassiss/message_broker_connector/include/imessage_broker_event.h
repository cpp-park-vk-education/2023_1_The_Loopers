#ifndef _IMESSAGEBROKEREVENT_H_
#define _IMESSAGEBROKEREVENT_H_

#include <functional>
#include <memory>
#include <string>

#include "iclient_session.h"
#include "icommon_connection.h"
#include "global.h"

class IMessageBrokerSignal {
  public:
    virtual ~IMessageBrokerSignal() = default;

    virtual void SetCommonConnection(ICommonConnection*)) {
    }

    virtual void SetDoOnNotified(std::function <
                                 void(
                                         int, const std::string&,
                                         ClientSession* >
                                         = [](int, const std::string&, IClientSession*) {})) {
    }

    virtual void Publish(int event, const std::string&, ServiceType = ServiceType::kNone) {
    }

    virtual void Subscribe(int event, Endpoint) {
    }

  protected:
    std::shared_ptr<ICommonConnection> m_connectionToMsgBroker;
    std::function <void(int, const std::string&,ClientSession* > m_doOnNotified;
};

#endif  // _IMESSAGEBROKEREVENT_H_
