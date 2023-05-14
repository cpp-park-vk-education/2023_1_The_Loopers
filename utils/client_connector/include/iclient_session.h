#ifndef _ICLIENTSESSION_H_
#define _ICLIENTSESSION_H_

#include <functional>
#include <string>

#include "inetwork_adapter.h"

namespace inklink_client_session
{
class IClientSession
{
public:
    virtual ~IClientSession() = default;
    // virtual void SetNetworkAdapter(INetworkStreamAdapter*)
    // {
    // }

    virtual void run_async(const std::string& host, unsigned short port)
    {
    }
    virtual void send(const std::string&)
    {
    }
    virtual void close()
    {
    }
};
}  // namespace inklink_client_session

#endif  // _ICLIENTSESSION_H_
