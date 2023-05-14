#pragma once

#include "isessions_factory.h"

#include <memory>
#include <string>


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
