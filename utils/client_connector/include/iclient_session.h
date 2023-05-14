#pragma once

#include "inetwork_adapter.h"

#include <functional>
#include <string>

namespace inklink::client_connector
{
class IClientSession
{
public:
    IClientSession(INetworkStreamAdapter&);
    virtual ~IClientSession() = default;

    virtual void run(const std::string& host, unsigned short port, const std::string& onHandshakeText) = 0;
    virtual void run_async(const std::string& host, unsigned short port, const std::string& onHandshakeText) = 0;
    virtual std::string sync_read() = 0;
    virtual void send(const std::string&) = 0;
    virtual void close() = 0;
};
} // namespace inklink::client_connector
