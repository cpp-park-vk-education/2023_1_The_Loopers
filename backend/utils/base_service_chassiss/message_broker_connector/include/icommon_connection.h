#pragma once

#include "global.h"
#include "iclient_session.h"

#include <memory>
#include <string>


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
