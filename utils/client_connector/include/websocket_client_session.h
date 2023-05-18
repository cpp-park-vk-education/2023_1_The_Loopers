#pragma once

#include "global_websocket_client_session.h"
#include "iclient_session.h"

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/system/error_code.hpp>

#include <deque>
#include <functional>
#include <memory>
#include <string>

namespace inklink::client_connector
{
namespace net = boost::asio;
namespace beast = boost::beast;

template <ConnectTypeErrorCodeCallbackConcept ConnectCallback =
                  std::function<void(ConnectType, boost::system::error_code, IClientSession*)>,
          StringErrorCodeCallbackConcept ReadCallback =
                  std::function<void(const std::string&, boost::system::error_code, IClientSession*)>,
          ErrorCodeCallbackConcept WriteCallback = std::function<void(boost::system::error_code)>,
          ErrorCodeCallbackConcept CloseCallback = std::function<void(boost::system::error_code)>>
class WebsocketClientSession final
        : public IClientSession,
          public std::enable_shared_from_this<
                  WebsocketClientSession<ConnectCallback, ReadCallback, WriteCallback, CloseCallback>>
{
public:
    WebsocketClientSession() = delete;

    // clang-format off
    explicit WebsocketClientSession(
            boost::asio::io_context&, 
            ConnectCallback = [](ConnectType, boost::system::error_code) {},
            ReadCallback = [](const std::string&, boost::system::error_code) {},
            WriteCallback = [](boost::system::error_code) {}, 
            CloseCallback = [](boost::system::error_code) {});
    // clang-format on

    WebsocketClientSession(const WebsocketClientSession&) = delete;
    WebsocketClientSession(WebsocketClientSession&&) = delete;

    WebsocketClientSession& operator=(const WebsocketClientSession&) = delete;
    WebsocketClientSession& operator=(WebsocketClientSession&&) = delete;

    ~WebsocketClientSession() final;

    void RunAsync(const std::string& host, uint16_t port) final;
    void Send(const std::string&) final;
    void Close() final;

private:
    using error_code = boost::system::error_code;

    void DoRead();

    void OnResolve(error_code ec, net::ip::tcp::resolver::results_type results);
    void OnConnect(error_code ec, net::ip::tcp::resolver::results_type::endpoint_type ep);
    void OnHandshake(error_code ec);
    void OnWrite(error_code ec, std::size_t bytes_transferred);
    void OnRead(error_code ec, std::size_t bytes_transferred);
    void OnClose(error_code ec);

    std::string m_host;
    net::ip::tcp::resolver m_resolver;
    beast::websocket::stream<beast::tcp_stream> m_websocketStream;

    bool m_close{false};
    bool m_writing{false};

    beast::flat_buffer m_readBuffer;
    std::deque<std::shared_ptr<std::string const>> m_sendQueue;

    ConnectCallback m_connectCallback;
    ReadCallback m_readCallback;
    WriteCallback m_writeCallback;
    CloseCallback m_closeCallback;
};
} // namespace inklink::client_connector
