#ifndef _WEBSOCKETSERVICESESSION_H_
#define _WEBSOCKETSERVICESESSION_H_

#include "boost/asio/buffer.hpp"
#include "boost/beast/core/flat_buffer.hpp"
#include "iservice_session.h"

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/system/error_code.hpp>
#include <deque>
#include <memory>
#include <string>

namespace inklink::server_network
{
namespace net = boost::asio;
namespace websocket = boost::beast::websocket;

template <typename T>
concept Do_ErrorCodeAndSession_Concept = requires(T&& t, boost::system::error_code ec, IServiceSession* session) {
    {
        std::forward<T>(t)(ec, session)
    } -> std::same_as<void>;
};

template <typename T>
concept Do_StringErrorCodeSession_Concept =
        requires(T&& t, const std::string& msgBody, boost::system::error_code ec, IServiceSession* session) {
            {
                std::forward<T>(t)(msgBody, ec, session)
            } -> std::same_as<void>;
        };

template <typename T>
concept Do_ErrorCode_Concept = requires(T&& t, boost::system::error_code ec) {
    {
        std::forward<T>(t)(ec)
    } -> std::same_as<void>;
};

using Fun_StringErrorCodeSession = std::function<void(const std::string&, boost::system::error_code, IServiceSession*)>;
using Fun_ErrorCodeAndSession = std::function<void(boost::system::error_code, IServiceSession*)>;
using Fun_ErrorCode = std::function<void(boost::system::error_code)>;

template <Do_StringErrorCodeSession_Concept DoOnRead = Fun_StringErrorCodeSession,
          Do_ErrorCodeAndSession_Concept DoOnAccept = Fun_ErrorCodeAndSession,
          Do_ErrorCode_Concept DoOnWrite = Fun_ErrorCode>
class WebsocketServiceSession
        : public IServiceSession,
          public std::enable_shared_from_this<WebsocketServiceSession<DoOnRead, DoOnAccept, DoOnWrite>>
{
    using IAuthorizer = authorizer::IAuthorizer;
    using InternalSessionsManager = base_service_chassis::InternalSessionsManager;
    using tcp = net::ip::tcp;

public:
    using error_code = boost::system::error_code;
    explicit WebsocketServiceSession(
            std::shared_ptr<InternalSessionsManager>, std::shared_ptr<IAuthorizer>, net::ip::tcp::socket&&,
            DoOnRead = [](const std::string&, error_code, IServiceSession*) {},
            DoOnAccept = [](error_code, IServiceSession*) {}, DoOnWrite = [](error_code) {}) noexcept;

    WebsocketServiceSession() = delete;

    WebsocketServiceSession(const WebsocketServiceSession&) = delete;
    WebsocketServiceSession(WebsocketServiceSession&&) = delete;

    WebsocketServiceSession& operator=(const WebsocketServiceSession&) = delete;
    WebsocketServiceSession& operator=(WebsocketServiceSession&&) = delete;

    ~WebsocketServiceSession() final;

    void RunAsync() final;

    void Send(const std::string&) final;

private:
    void DoRead();

    void OnRun();
    void OnAccept(boost::system::error_code ec);
    void OnRead(boost::system::error_code ec, std::size_t bytes_transferred);
    void OnWrite(boost::system::error_code ec, std::size_t bytes_transferred);

    Endpoint m_endpoint;

    websocket::stream<tcp::socket> m_ws;
    boost::beast::multi_buffer m_buffer;
    std::deque<std::shared_ptr<std::string const>> m_queue;

    DoOnRead m_doOnRead;
    DoOnAccept m_doOnAccept;
    DoOnWrite m_doOnWrite;
};
} // namespace inklink::server_network

#endif // _WEBSOCKETSERVICESESSION_H_
