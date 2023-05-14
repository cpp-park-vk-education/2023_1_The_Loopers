#pragma once

#include "global.h"

#include <string>
#include <vector>


class ILocalServiceRegistry
{
public:
    virtual ~ILocalServiceRegistry() = default;

    virtual void AddService(ServiceType, const Endpoint&)
    {
    }

    virtual std::vector<Endpoint> GetServices(ServiceType)
    {
    }
};
