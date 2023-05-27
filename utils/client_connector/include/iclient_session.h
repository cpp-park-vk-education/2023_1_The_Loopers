#pragma once

#include "inklink_global.h"

#include <functional>
#include <string>

namespace inklink::client_connector
{
class IClientSession
{
public:
    virtual ~IClientSession() = default;

    // virtual void run(const std::string& host, uint16_t port, const std::string& onHandshakeText) = 0;
    virtual void RunAsync(const std::string& host, uint16_t port) = 0;
    // virtual std::string sync_read() = 0;
    virtual void Send(const std::string&) = 0;
    virtual void Close() = 0;

    virtual Endpoint GetSelfEndpoint() = 0;
};
} // namespace inklink::client_connector
