#pragma once

#include "global.h"

#include <string>
#include <vector>

namespace inklink::base_service_chassis
{
class ILocalServiceRegistry
{
public:
    virtual ~ILocalServiceRegistry() = default;

    virtual void AddService(ServiceType, const Endpoint&) = 0;

    virtual std::vector<Endpoint> GetServices(ServiceType) = 0;
};
} // namespace inklink::base_service_chassis
