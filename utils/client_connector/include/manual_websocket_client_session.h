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

    const std::string kClientName{"inklink_manual_client"};
};
} // namespace inklink::client_connector

namespace inklink::client_connector
{
// clang-format off
template <ConnectTypeErrorCodeCallbackConcept ConnectCallback, StringErrorCodeCallbackConcept ReadCallback,
          ErrorCodeCallbackConcept WriteCallback, ErrorCodeCallbackConcept CloseCallback> 
inline ManualWebsocketClientSession<ConnectCallback, ReadCallback, WriteCallback, 
                                    CloseCallback>::ManualWebsocketClientSession(
        boost::asio::io_context& ioc, 
        const std::string& address, unsigned short port, 
        ConnectCallback connectCallback,
        ReadCallback readCallback, 
        WriteCallback writeCallback, 
        CloseCallback closeCallback)
        : m_timer{ioc}, 
          m_connectCallback{connectCallback}, 
          m_readCallback{readCallback},
          m_writeCallback{writeCallback}, 
          m_closeCallback{closeCallback}
{
    const boost::asio::ip::tcp::endpoint localEndpoint(boost::asio::ip::address::from_string(address), port);
    // do not really need strand here, and I'm not sure can be socket constructed using strand
    m_socket = boost::asio::ip::tcp::socket(ioc, localEndpoint);
}
// clang-format on

template <ConnectTypeErrorCodeCallbackConcept ConnectCallback, StringErrorCodeCallbackConcept ReadCallback,
          ErrorCodeCallbackConcept WriteCallback, ErrorCodeCallbackConcept CloseCallback>
inline ManualWebsocketClientSession<ConnectCallback, ReadCallback, WriteCallback,
                                    CloseCallback>::~ManualWebsocketClientSession()
{
    m_close = true;
    if (m_websocketStream->is_open())
    {
        while (m_writing.load())
        {
            continue;
        }
        error_code ec{};
        m_websocketStream->close(boost::beast::websocket::close_code::normal, ec);
        m_closeCallback(ec);
    }

    delete m_websocketStream;
    m_websocketStream = nullptr;
}

template <ConnectTypeErrorCodeCallbackConcept ConnectCallback, StringErrorCodeCallbackConcept ReadCallback,
          ErrorCodeCallbackConcept WriteCallback, ErrorCodeCallbackConcept CloseCallback>
inline void ManualWebsocketClientSession<ConnectCallback, ReadCallback, WriteCallback, CloseCallback>::RunAsync(
        const std::string& host, unsigned short port)
{
    m_host = host + ':' + std::to_string(port);

    // socket must be connected manually: asio::connect reset local endpoint bind
    boost::asio::ip::tcp::endpoint serverEndpoint(boost::asio::ip::address::from_string(host), port);
    m_socket.async_connect(serverEndpoint, [this](error_code errCode) { this->OnConnect(errCode); });

    // in case of no connection with internet etc.
    m_timer.expires_from_now(boost::posix_time::seconds(30));
    // Set up the timer's expiration handler
    m_timer.async_wait(
            [this](const error_code& errCode)
            {
                if (errCode != boost::asio::error::operation_aborted)
                {
                    // Timeout occurred, cancel the socket's asynchronous operations
                    m_socket.cancel();
                    // now on connect will be called
                }
            });
}

template <ConnectTypeErrorCodeCallbackConcept ConnectCallback, StringErrorCodeCallbackConcept ReadCallback,
          ErrorCodeCallbackConcept WriteCallback, ErrorCodeCallbackConcept CloseCallback>
inline void ManualWebsocketClientSession<ConnectCallback, ReadCallback, WriteCallback, CloseCallback>::Send(
        const std::string& msgBody)
{
    auto ss = std::make_shared<std::string const>(msgBody);
    // Always add to queue
    m_sendQueue.push_back(ss);
    m_writing = true;

    // Are we already writing?
    if (m_sendQueue.size() > 1)
    {
        return;
    }

    // We are not currently writing, so send this immediately
    m_websocketStream->async_write(
            boost::asio::buffer(*m_sendQueue.front()),
            boost::beast::bind_front_handler(&ManualWebsocketClientSession::OnWrite, this->shared_from_this()));
}

template <ConnectTypeErrorCodeCallbackConcept ConnectCallback, StringErrorCodeCallbackConcept ReadCallback,
          ErrorCodeCallbackConcept WriteCallback, ErrorCodeCallbackConcept CloseCallback>
inline void ManualWebsocketClientSession<ConnectCallback, ReadCallback, WriteCallback, CloseCallback>::Close()
{
    m_close = true;

    if (!m_writing.load())
    {
        m_websocketStream->async_close(
                boost::beast::websocket::close_code::normal,
                boost::beast::bind_front_handler(&ManualWebsocketClientSession::OnClose, this->shared_from_this()));
    }
}

template <ConnectTypeErrorCodeCallbackConcept ConnectCallback, StringErrorCodeCallbackConcept ReadCallback,
          ErrorCodeCallbackConcept WriteCallback, ErrorCodeCallbackConcept CloseCallback>
inline void ManualWebsocketClientSession<ConnectCallback, ReadCallback, WriteCallback, CloseCallback>::DoRead()
{
    if (!m_close.load())
    {
        // Read a message into our buffer
        m_websocketStream->async_read(
                m_readBuffer,
                boost::beast::bind_front_handler(&ManualWebsocketClientSession::OnRead, this->shared_from_this()));
    }
}

template <ConnectTypeErrorCodeCallbackConcept ConnectCallback, StringErrorCodeCallbackConcept ReadCallback,
          ErrorCodeCallbackConcept WriteCallback, ErrorCodeCallbackConcept CloseCallback>
inline void
ManualWebsocketClientSession<ConnectCallback, ReadCallback, WriteCallback, CloseCallback>::OnConnect(error_code ec)
{
    m_timer.cancel();
    m_connectCallback(ConnectType::kConnect, ec);
    if (ec)
    {
        return;
    }

    m_websocketStream = new boost::beast::websocket::stream<boost::beast::tcp_stream>(std::move(m_socket));

    // Turn off the timeout on the tcp_stream, because
    // the websocket stream has its own timeout system.
    boost::beast::get_lowest_layer(*m_websocketStream).expires_never();

    // Set suggested timeout settings for the websocket
    m_websocketStream->set_option(
            boost::beast::websocket::stream_base::timeout::suggested(boost::beast::role_type::client));

    // Set a decorator to change the User-Agent of the handshake
    m_websocketStream->set_option(boost::beast::websocket::stream_base::decorator(
            [this](boost::beast::websocket::request_type& req)
            { req.set(boost::beast::http::field::user_agent, kClientName); }));

    // Perform the websocket handshake
    m_websocketStream->async_handshake(
            m_host, "/",
            boost::beast::bind_front_handler(&ManualWebsocketClientSession::OnHandshake, this->shared_from_this()));
}

template <ConnectTypeErrorCodeCallbackConcept ConnectCallback, StringErrorCodeCallbackConcept ReadCallback,
          ErrorCodeCallbackConcept WriteCallback, ErrorCodeCallbackConcept CloseCallback>
inline void
ManualWebsocketClientSession<ConnectCallback, ReadCallback, WriteCallback, CloseCallback>::OnHandshake(error_code ec)
{
    m_connectCallback(ConnectType::kHandshake, ec);
    if (ec)
    {
        return;
    }

    // set binary format because don't know wich format will be used
    m_websocketStream->binary(true);

    DoRead();
}

template <ConnectTypeErrorCodeCallbackConcept ConnectCallback, StringErrorCodeCallbackConcept ReadCallback,
          ErrorCodeCallbackConcept WriteCallback, ErrorCodeCallbackConcept CloseCallback>
inline void
ManualWebsocketClientSession<ConnectCallback, ReadCallback, WriteCallback, CloseCallback>::OnWrite(error_code ec,
                                                                                                   std::size_t)
{
    m_writeCallback(ec);

    if (ec)
    {
        return;
    }

    // Remove the string from the queue
    m_sendQueue.pop_front();

    if (m_close)
    {
        m_sendQueue.clear();
        m_websocketStream->async_close(
                boost::beast::websocket::close_code::normal,
                boost::beast::bind_front_handler(&ManualWebsocketClientSession::OnClose, this->shared_from_this()));
    }

    // Send the next message if any
    if (!m_sendQueue.empty())
    {
        m_websocketStream->async_write(
                boost::asio::buffer(*m_sendQueue.front()),
                boost::beast::bind_front_handler(&ManualWebsocketClientSession::OnWrite, this->shared_from_this()));
    }
    else
    {
        m_writing = false;
    }
}

template <ConnectTypeErrorCodeCallbackConcept ConnectCallback, StringErrorCodeCallbackConcept ReadCallback,
          ErrorCodeCallbackConcept WriteCallback, ErrorCodeCallbackConcept CloseCallback>
inline void
ManualWebsocketClientSession<ConnectCallback, ReadCallback, WriteCallback, CloseCallback>::OnRead(error_code ec,
                                                                                                  std::size_t)
{
    m_readCallback(boost::beast::buffers_to_string(m_readBuffer.data()), ec);
    if (ec)
    {
        return;
    }

    m_readBuffer.consume(m_readBuffer.size());

    DoRead();
}

template <ConnectTypeErrorCodeCallbackConcept ConnectCallback, StringErrorCodeCallbackConcept ReadCallback,
          ErrorCodeCallbackConcept WriteCallback, ErrorCodeCallbackConcept CloseCallback>
inline void
ManualWebsocketClientSession<ConnectCallback, ReadCallback, WriteCallback, CloseCallback>::OnClose(error_code ec)
{
    m_closeCallback(ec);
    if (ec)
    {
        return;
    }
}
} // namespace inklink::client_connector
