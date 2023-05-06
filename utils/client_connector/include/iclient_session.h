#ifndef _ICLIENTSESSION_H_
#define _ICLIENTSESSION_H_

#include <functional>
#include <string>

#include "inetwork_adapter.h"

class IClientSession {
  public:
    virtual ~IClientSession() = default;
    virtual void SetNetworkAdapter(NetworkStreamAdapter*) {
    }

    virtual void run(const std::string& host, unsigned short port,
                     const std::string& onHandshakeText) {
    }
    virtual void run_async(const std::string& host, unsigned short port,
                           const std::string& onHandshakeText) {
    }
    virtual std::string sync_read() {
    }
    virtual void send(const std::string&) {
    }
    virtual void close() {
    }
};

#endif  // _ICLIENTSESSION_H_
