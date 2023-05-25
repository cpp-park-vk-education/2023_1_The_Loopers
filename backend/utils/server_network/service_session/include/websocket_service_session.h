#pragma once

#include "inklink/server_network/callback_concepts.h"
#include "iservice_session.h"

#include <internal_sessions_manager.h>

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/system/error_code.hpp>

#include <algorithm>
#include <concepts>
#include <deque>
#include <functional>
#include <memory>
#include <string>

namespace inklink::server_network
{
namespace net = boost::asio;
namespace websocket = boost::beast::websocket;

template <StringErrorCodeSessionCallbackConcept ReadCallback =
                  std::function<void(const std::string&, boost::system::error_code, IServiceSession*)>,
          ErrorCodeAndSessionCallbackConcept AcceptCallback =
                  std::function<void(boost::system::error_code, IServiceSession*)>,
          ErrorCodeCallbackConcept WriteCallback = std::function<void(boost::system::error_code, IServiceSession*)>>
class WebsocketServiceSession
        : public IServiceSession,
          public std::enable_shared_from_this<WebsocketServiceSession<ReadCallback, AcceptCallback, WriteCallback>>
{
    using IAuthorizer = authorizer::IAuthorizer;
    using InternalSessionsManager = base_service_chassis::InternalSessionsManager;
    using tcp = net::ip::tcp;

public:
    using stream_base = boost::beast::websocket::stream_base;
    using error_code = boost::system::error_code;
    // clang-format off
    explicit WebsocketServiceSession(
            std::shared_ptr<InternalSessionsManager>, 
            std::shared_ptr<IAuthorizer>, 
            net::ip::tcp::socket&&,
            ReadCallback = [](const std::string&, error_code, IServiceSession*) {},
            AcceptCallback = [](error_code, IServiceSession*) {}, 
            WriteCallback = [](error_code, IServiceSession*) {}) noexcept;
    // clang-format on

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

    websocket::stream<tcp::socket> m_websocketStream;
    boost::beast::multi_buffer m_readBuffer;
    std::deque<std::shared_ptr<std::string const>> m_sendQueue;

    ReadCallback m_readCallback;
    AcceptCallback m_acceptCallback;
    WriteCallback m_writeCallback;
};
} // namespace inklink::server_network

namespace inklink::server_network
{
namespace beast = boost::beast;
// clang-format off
template <StringErrorCodeSessionCallbackConcept ReadCallback, ErrorCodeAndSessionCallbackConcept AcceptCallback,
          ErrorCodeCallbackConcept WriteCallback> inline
WebsocketServiceSession<ReadCallback, AcceptCallback, WriteCallback>::WebsocketServiceSession(
            std::shared_ptr<InternalSessionsManager> manager, 
            std::shared_ptr<IAuthorizer> auth, 
            net::ip::tcp::socket&& socket,
            ReadCallback readCallback,
            AcceptCallback acceptCallback, 
            WriteCallback writeCallback) noexcept
        : IServiceSession{manager, auth}, 
          m_endpoint{.address = socket.remote_endpoint().address().to_string(), 
                     .port = socket.remote_endpoint().port()},
          m_websocketStream{std::move(socket)}, 
          m_readCallback{readCallback}, 
          m_acceptCallback{acceptCallback},
          m_writeCallback{writeCallback}
{
}
// clang-format on

template <StringErrorCodeSessionCallbackConcept ReadCallback, ErrorCodeAndSessionCallbackConcept AcceptCallback,
          ErrorCodeCallbackConcept WriteCallback>
inline WebsocketServiceSession<ReadCallback, AcceptCallback, WriteCallback>::~WebsocketServiceSession()
{
    m_manager->RemoveSession(this);
}

template <StringErrorCodeSessionCallbackConcept ReadCallback, ErrorCodeAndSessionCallbackConcept AcceptCallback,
          ErrorCodeCallbackConcept WriteCallback>
inline void WebsocketServiceSession<ReadCallback, AcceptCallback, WriteCallback>::RunAsync()
{
    // We need to be executing within a strand to perform async operations
    // on the I/O objects in this session. Although not strictly necessary
    // for single-threaded contexts, this example code is written to be
    // thread-safe by default.
    net::dispatch(m_websocketStream.get_executor(),
                  beast::bind_front_handler(&WebsocketServiceSession::OnRun, this->shared_from_this()));
}

template <StringErrorCodeSessionCallbackConcept ReadCallback, ErrorCodeAndSessionCallbackConcept AcceptCallback,
          ErrorCodeCallbackConcept WriteCallback>
inline void WebsocketServiceSession<ReadCallback, AcceptCallback, WriteCallback>::OnRun()
{
    // Set suggested timeout settings for the websocket
    m_websocketStream.set_option(stream_base::timeout::suggested(beast::role_type::server));

    // Set a decorator to change the Server of the handshake
    const std::string_view kServerName{"inklink-simultaneous-access-service"};
    m_websocketStream.set_option(stream_base::decorator([kServerName](websocket::response_type& res)
                                                        { res.set(beast::http::field::server, kServerName); }));
    // Accept the websocket handshake
    m_websocketStream.async_accept(
            beast::bind_front_handler(&WebsocketServiceSession::OnAccept, this->shared_from_this()));
}

template <StringErrorCodeSessionCallbackConcept ReadCallback, ErrorCodeAndSessionCallbackConcept AcceptCallback,
          ErrorCodeCallbackConcept WriteCallback>
inline void WebsocketServiceSession<ReadCallback, AcceptCallback, WriteCallback>::Send(const std::string& message)
{
    auto ss = std::make_shared<std::string const>(message);
    // Always add to queue
    m_sendQueue.push_back(ss);

    // Are we already writing?
    if (m_sendQueue.size() > 1)
    {
        return;
    }

    // We are not currently writing, so send this immediately
    m_websocketStream.async_write(
            net::buffer(*m_sendQueue.front()),
            beast::bind_front_handler(&WebsocketServiceSession::OnWrite, this->shared_from_this()));
}

template <StringErrorCodeSessionCallbackConcept ReadCallback, ErrorCodeAndSessionCallbackConcept AcceptCallback,
          ErrorCodeCallbackConcept WriteCallback>
inline void WebsocketServiceSession<ReadCallback, AcceptCallback, WriteCallback>::OnAccept(boost::system::error_code ec)
{
    m_acceptCallback(ec, this);

    if (ec)
    {
        // destructor is called automatically (see file doxygen comment)
        return;
    }

    // set binary format because don't know wich format will be used
    m_websocketStream.binary(true);

    // Read a message
    DoRead();
}

template <StringErrorCodeSessionCallbackConcept ReadCallback, ErrorCodeAndSessionCallbackConcept AcceptCallback,
          ErrorCodeCallbackConcept WriteCallback>
inline void WebsocketServiceSession<ReadCallback, AcceptCallback, WriteCallback>::DoRead()
{
    // Read a message into our buffer
    m_websocketStream.async_read(m_readBuffer,
                                 beast::bind_front_handler(&WebsocketServiceSession::OnRead, this->shared_from_this()));
}

template <StringErrorCodeSessionCallbackConcept ReadCallback, ErrorCodeAndSessionCallbackConcept AcceptCallback,
          ErrorCodeCallbackConcept WriteCallback>
inline void WebsocketServiceSession<ReadCallback, AcceptCallback, WriteCallback>::OnRead(boost::system::error_code ec,
                                                                                         std::size_t)
{
    m_readCallback(beast::buffers_to_string(m_readBuffer.data()), ec, this);

    // TODO (a.novak) some errors should be ok and session should not be destructed, but in my
    // practice connection should be closed not only after websocket::error::closed (at least in
    // version 1.72)
    if (ec)
    {
        // destructor is called automatically (see file doxygen comment)
        return;
    }

    // clean buffer: if there was smth extra, it was garbage (it supposed to
    // contain nothing before read)
    m_readBuffer.consume(m_readBuffer.size());

    DoRead();
}

template <StringErrorCodeSessionCallbackConcept ReadCallback, ErrorCodeAndSessionCallbackConcept AcceptCallback,
          ErrorCodeCallbackConcept WriteCallback>
inline void WebsocketServiceSession<ReadCallback, AcceptCallback, WriteCallback>::OnWrite(boost::system::error_code ec,
                                                                                          std::size_t)
{
    // remove send string from queue
    m_sendQueue.pop_front();

    m_writeCallback(ec, this);
    if (ec)
    {
        return;
    }

    // Send the next message if any
    if (!m_sendQueue.empty())
    {
        m_websocketStream.async_write(
                net::buffer(*m_sendQueue.front()),
                beast::bind_front_handler(&WebsocketServiceSession::OnWrite, this->shared_from_this()));
    }
}
} // namespace inklink::server_network
