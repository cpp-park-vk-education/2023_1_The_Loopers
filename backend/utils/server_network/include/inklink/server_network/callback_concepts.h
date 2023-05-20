#pragma once

#include <boost/system/error_code.hpp>

#include <concepts>
#include <string>
#include <utility>

namespace inklink::server_network
{
class IServiceSession;

template <typename T>
concept ErrorCodeAndSessionCallbackConcept = requires(T&& t, boost::system::error_code ec, IServiceSession* session) {
    {
        std::forward<T>(t)(ec, session)
    } -> std::same_as<void>;
};

template <typename T>
concept StringErrorCodeSessionCallbackConcept =
        requires(T&& t, const std::string& msgBody, boost::system::error_code ec, IServiceSession* session) {
            {
                std::forward<T>(t)(msgBody, ec, session)
            } -> std::same_as<void>;
        };

template <typename T>
concept ErrorCodeCallbackConcept = requires(T&& t, boost::system::error_code ec) {
    {
        std::forward<T>(t)(ec)
    } -> std::same_as<void>;
};
} // namespace inklink::server_network
