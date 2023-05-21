#pragma once

#include "iclient_session.h"
#include "websocket_fwd.h"

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/system/error_code.hpp>

#include <atomic>
#include <deque>
#include <functional>
#include <memory>
#include <string>

namespace inklink::client_connector
{
template <ConnectTypeErrorCodeCallbackConcept ConnectCallback =
                  std::function<void(ConnectType, boost::system::error_code, IClientSession*)>,
          StringErrorCodeCallbackConcept ReadCallback =
                  std::function<void(const std::string&, boost::system::error_code, IClientSession*)>,
          ErrorCodeCallbackConcept WriteCallback = std::function<void(boost::system::error_code)>,
          ErrorCodeCallbackConcept CloseCallback = std::function<void(boost::system::error_code)>>
class ManualWebsocketClientSession final
        : public IClientSession,
          public std::enable_shared_from_this<
                  ManualWebsocketClientSession<ConnectCallback, ReadCallback, WriteCallback, CloseCallback>>
{
public:
    ManualWebsocketClientSession() = delete;

    // clang-format off
    ManualWebsocketClientSession(
            boost::asio::io_context&, 
            const std::string& address, unsigned short port,
            ConnectCallback = [](ConnectType, boost::system::error_code, IClientSession*) {},
            ReadCallback = [](const std::string&, boost::system::error_code, IClientSession*) {},
            WriteCallback = [](boost::system::error_code) {}, 
            CloseCallback = [](boost::system::error_code) {});
    // clang-format on

    ManualWebsocketClientSession(const ManualWebsocketClientSession&) = delete;
    ManualWebsocketClientSession(ManualWebsocketClientSession&&) = delete;

    ManualWebsocketClientSession& operator=(const ManualWebsocketClientSession&) = delete;
    ManualWebsocketClientSession& operator=(ManualWebsocketClientSession&&) = delete;

    ~ManualWebsocketClientSession() override;

    void RunAsync(const std::string& host, unsigned short port) override;
    void Send(const std::string&) override;
    void Close() override;

private:
    using error_code = boost::system::error_code;

    void DoRead();

    void OnConnect(error_code ec);
    void OnHandshake(error_code ec);
    void OnWrite(error_code ec, std::size_t bytes_transferred);
    void OnRead(error_code ec, std::size_t bytes_transferred);
    void OnClose(error_code ec);

    boost::asio::ip::tcp::socket m_socket;
    boost::asio::deadline_timer m_timer;

    std::atomic_bool m_close{false};
    std::atomic_bool m_writing{false};

    std::string m_host;
    boost::beast::websocket::stream<boost::beast::tcp_stream>* m_websocketStream; // because operator= deleted

    boost::beast::flat_buffer m_readBuffer;
    std::deque<std::shared_ptr<std::string const>> m_sendQueue;

    ConnectCallback m_connectCallback;
    ReadCallback m_readCallback;
    WriteCallback m_writeCallback;
    CloseCallback m_closeCallback;
};
} // namespace inklink::client_connector
