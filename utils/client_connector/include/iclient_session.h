#pragma once

#include "inetwork_adapter.h"

#include <functional>
#include <string>


class IClientSession
{
public:
    virtual ~IClientSession() = default;
    virtual void SetNetworkAdapter(INetworkStreamAdapter*)
    {
    }

    virtual void run(const std::string& host, unsigned short port, const std::string& onHandshakeText)
    {
    }
    virtual void run_async(const std::string& host, unsigned short port, const std::string& onHandshakeText)
    {
    }
    virtual std::string sync_read()
    {
    }
    virtual void send(const std::string&)
    {
    }
    virtual void close()
    {
    }
};
