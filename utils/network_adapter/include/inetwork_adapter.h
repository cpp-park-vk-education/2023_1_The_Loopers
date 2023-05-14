#pragma once

#include <boost/system/error_code.hpp>
#include <functional>
#include <string>
#include <string_view>

namespace inklink::network_adapter
{
class INetworkStreamAdapter
{
public:
    virtual ~INetworkStreamAdapter() = default;

    virtual void async_handshake(
            std::string_view host, std::string_view target,
            std::function<void(boost::system::error_code)> = [](boost::system::error_code) {}) = 0;

    virtual void async_read(std::function<void(std::string, boost::system::error_code, size_t)> =
                                    [](std::string, boost::system::error_code, size_t) {}) = 0;

    virtual void async_write(
            std::string,
            std::function<void(boost::system::error_code, size_t)> = [](boost::system::error_code, size_t) {}) = 0;

    virtual void async_accept(std::function<void(boost::system::error_code)> = [](boost::system::error_code) {}) = 0;

    virtual void async_close(std::function<void(boost::system::error_code)> = [](boost::system::error_code) {}) = 0;
};
} // namespace inklink::network_adapter
