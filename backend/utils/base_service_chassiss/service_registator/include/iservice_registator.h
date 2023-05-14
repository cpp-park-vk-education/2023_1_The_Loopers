#pragma once

#include "global.h"

#include <memory>
#include <string>
#include <vector>


class IClientSession;

class IServiceRegistrator
{
public:
    virtual ~IServiceRegistrator() = default;

    virtual void SetConnection(IClientSession*)
    {
    }

    virtual bool Register(ServiceType, const Endpoint&)
    {
    }
    virtual void Deregister(ServiceType, const Endpoint&)
    {
    }
    virtual std::vector<Endpoint> GetEndpoints(ServiceType)
    {
    }

protected:
    std::shared_ptr<IClientSession> m_cnnectionToRegistry;
};
