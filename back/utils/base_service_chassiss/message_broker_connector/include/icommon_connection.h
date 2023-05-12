#ifndef _ICOMMONCONNECTION_H_
#define _ICOMMONCONNECTION_H_

#include <memory>
#include <string>

#include "global.h"
#include "iclient_session.h"

class ICommonConnection
{
public:
    virtual ~ICommonConnection() = default;

    virtual void Init(ServiceType, const Endpoint& self, const Endpoint& other)
    {
    }

    virtual void ChangeConnection(ServiceType, const Endpoint& self, const Endpoint& other)
    {
    }

    virtual std::shared_ptr<IClientSession> GetSession()
    {
    }

protected:
    std::shared_ptr<IClientSession> m_session;
    bool connected{false};
    ServiceType m_serviceType;
};

#endif  // _ICOMMONCONNECTION_H_
