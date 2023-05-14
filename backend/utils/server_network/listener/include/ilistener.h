#pragma once

#include "isessions_factory.h"

#include <memory>
#include <string>

namespace inklink::server_network
{
class IListener
{
public:
    explicit IListener(ISessionsFactory&) noexcept;
    virtual ~IListener() = default;

    virtual void AsyncRun() = 0;

protected:
    ISessionsFactory& m_factory;
};
} // namespace inklink::server_network
