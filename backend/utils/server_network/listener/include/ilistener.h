#pragma once

#include "isessions_factory.h"

#include <memory>
#include <string>

namespace inklink::server_network
{
class IListener
{
public:
    virtual ~IListener() = default;

    virtual void run() = 0;
    virtual void async_run() = 0;
    virtual void SetSessionFactory(ISessionsFactory*) = 0;
};
} // namespace inklink::server_network
