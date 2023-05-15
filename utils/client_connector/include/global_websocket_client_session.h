#pragma once

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/system/error_code.hpp>

namespace inklink::client_connector
{
class IClientSession;

enum ConnectType
{
    kResolve,
    kConnect,
    kHandshake
};

template <typename T>
concept Do_StringErrorCode_Concept =
        requires(T&& t, const std::string& msgBody, boost::system::error_code ec, IClientSession* session) {
            {
                std::forward<T>(t)(msgBody, ec, session)
            } -> std::same_as<void>;
        };

template <typename T>
concept Do_ConnectTypeErrorCode_Concept = requires(T&& t, ConnectType ct, boost::system::error_code ec) {
    {
        std::forward<T>(t)(ct, ec)
    } -> std::same_as<void>;
};

template <typename T>
concept Do_ErrorCode_Concept = requires(T&& t, boost::system::error_code ec) {
    {
        std::forward<T>(t)(ec)
    } -> std::same_as<void>;
};

using Fun_ConnectTypeErrorCodeSession = std::function<void(ConnectType, boost::system::error_code, IClientSession*)>;
using Fun_StringErrorCodeSession = std::function<void(const std::string&, boost::system::error_code, IClientSession*)>;
using Fun_ErrorCode = std::function<void(boost::system::error_code)>;

} // namespace inklink::client_connector
