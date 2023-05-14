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

    virtual void run()
    {
    }
    virtual void async_run()
    {
    }
    virtual void SetSessionFactory(ISessionsFactory*)
    {
    }
};
} // namespace inklink::server_network
