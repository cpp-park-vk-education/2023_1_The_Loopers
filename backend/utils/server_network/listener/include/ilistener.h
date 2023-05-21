#pragma once

#include "isessions_factory.h"

#include <memory>
#include <string>

namespace inklink::server_network
{
class IListener
{
public:
    explicit IListener(std::unique_ptr<ISessionsFactory> factory) noexcept : m_factory{std::move(factory)}
    {
    }
    virtual ~IListener() = default;

    virtual void AsyncRun() = 0;

protected:
    std::unique_ptr<ISessionsFactory> m_factory;
};
} // namespace inklink::server_network
