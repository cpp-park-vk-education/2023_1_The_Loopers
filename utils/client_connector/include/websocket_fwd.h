#pragma once

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/system/error_code.hpp>

#include <concepts>
#include <functional>

namespace inklink::client_connector
{
class IClientSession;

enum class ConnectType
{
    kResolve,
    kConnect,
    kHandshake
};

template <typename T>
concept StringErrorCodeCallbackConcept =
        requires(T&& t, const std::string& msgBody, boost::system::error_code ec, IClientSession* session) {
            {
                std::forward<T>(t)(msgBody, ec, session)
            } -> std::same_as<void>;
        };

template <typename T>
concept ConnectTypeErrorCodeCallbackConcept =
        requires(T&& t, ConnectType ct, boost::system::error_code ec, IClientSession* session) {
            {
                std::forward<T>(t)(ct, ec, session)
            } -> std::same_as<void>;
        };

template <typename T>
concept ErrorCodeCallbackConcept = requires(T&& t, boost::system::error_code ec) {
    {
        std::forward<T>(t)(ec)
    } -> std::same_as<void>;
};
} // namespace inklink::client_connector
