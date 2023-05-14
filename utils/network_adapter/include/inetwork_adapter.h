#pragma once

#include <boost/system/error_code.hpp>
#include <functional>
#include <string>
#include <string_view>

namespace inklink::network_adapter
{
class INetworkStreamAdapter
{
    using error_code = boost::system::error_code;

public:
    virtual ~INetworkStreamAdapter() = default;

    virtual void async_handshake(
            std::string_view host, std::string_view target, std::function<void(error_code)> = [](error_code) {}) = 0;
    virtual void async_read(std::function<void(std::string, error_code, size_t)> = [](std::string, error_code,
                                                                                      size_t) {}) = 0;
    virtual void async_write(
            std::string, std::function<void(error_code, size_t)> = [](error_code, size_t) {}) = 0;
    virtual void async_accept(std::function<void(error_code)> = [](error_code) {}) = 0;
    virtual void async_close(std::function<void(error_code)> = [](error_code) {}) = 0;
};
} // namespace inklink::network_adapter
