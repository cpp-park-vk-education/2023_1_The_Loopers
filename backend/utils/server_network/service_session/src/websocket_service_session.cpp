#include "websocket_service_session.h"

#include <internal_sessions_manager.h>

#include <algorithm>

namespace inklink::server_network
{
namespace beast = boost::beast;
// clang-format off
template <StringErrorCodeSessionCallbackConcept ReadCallback, ErrorCodeAndSessionCallbackConcept AcceptCallback,
          StringErrorCodeCallbackConcept WriteCallback>
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
          StringErrorCodeCallbackConcept WriteCallback>
WebsocketServiceSession<ReadCallback, AcceptCallback, WriteCallback>::~WebsocketServiceSession()
{
    m_manager->RemoveSession(this);
}

template <StringErrorCodeSessionCallbackConcept ReadCallback, ErrorCodeAndSessionCallbackConcept AcceptCallback,
          StringErrorCodeCallbackConcept WriteCallback>
void WebsocketServiceSession<ReadCallback, AcceptCallback, WriteCallback>::RunAsync()
{
    // We need to be executing within a strand to perform async operations
    // on the I/O objects in this session. Although not strictly necessary
    // for single-threaded contexts, this example code is written to be
    // thread-safe by default.
    net::dispatch(m_websocketStream.get_executor(),
                  beast::bind_front_handler(&WebsocketServiceSession::OnRun, this->shared_from_this()));
}

template <StringErrorCodeSessionCallbackConcept ReadCallback, ErrorCodeAndSessionCallbackConcept AcceptCallback,
          StringErrorCodeCallbackConcept WriteCallback>
void WebsocketServiceSession<ReadCallback, AcceptCallback, WriteCallback>::OnRun()
{
    // Set suggested timeout settings for the websocket
    m_websocketStream.set_option(websocket::stream_base::timeout::suggested(beast::role_type::server));

    // Set a decorator to change the Server of the handshake
    m_websocketStream.set_option(websocket::stream_base::decorator(
            [](websocket::response_type &res)
            {
                res.set(beast::http::field::server,
                        std::string(BOOST_BEAST_VERSION_STRING) + " inklink-simultaneous-access-service");
            }));
    // Accept the websocket handshake
    m_websocketStream.async_accept(
            beast::bind_front_handler(&WebsocketServiceSession::OnAccept, this->shared_from_this()));
}

template <StringErrorCodeSessionCallbackConcept ReadCallback, ErrorCodeAndSessionCallbackConcept AcceptCallback,
          StringErrorCodeCallbackConcept WriteCallback>
void WebsocketServiceSession<ReadCallback, AcceptCallback, WriteCallback>::Send(const std::string &message)
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
          StringErrorCodeCallbackConcept WriteCallback>
void WebsocketServiceSession<ReadCallback, AcceptCallback, WriteCallback>::OnAccept(boost::system::error_code ec)
{
    m_acceptCallback(ec, this);

    if (ec)
    {
        // destructor is called automatically, because no more shared_ptr points to this
        return;
    }

    // Read a message
    DoRead();
}

template <StringErrorCodeSessionCallbackConcept ReadCallback, ErrorCodeAndSessionCallbackConcept AcceptCallback,
          StringErrorCodeCallbackConcept WriteCallback>
void WebsocketServiceSession<ReadCallback, AcceptCallback, WriteCallback>::DoRead()
{
    // Read a message into our buffer
    m_websocketStream.async_read(m_readBuffer,
                                 beast::bind_front_handler(&WebsocketServiceSession::OnRead, this->shared_from_this()));
}

template <StringErrorCodeSessionCallbackConcept ReadCallback, ErrorCodeAndSessionCallbackConcept AcceptCallback,
          StringErrorCodeCallbackConcept WriteCallback>
void WebsocketServiceSession<ReadCallback, AcceptCallback, WriteCallback>::OnRead(boost::system::error_code ec,
                                                                                  std::size_t)
{
    m_readCallback(beast::buffers_to_string(m_readBuffer.data()), ec, this);

    // TODO (a.novak) some errors should be ok and session should not be destructed, but in my
    // practice connection should be closed not only after websocket::error::closed (at least in
    // version 1.72)
    if (ec)
    {
        // this will be deleted automatically, because no more shared_ptr will point to this
        return;
    }

    // clean buffer: if there was smth extra, it was garbage (it supposed to
    // contain nothing before read)
    m_readBuffer.consume(m_readBuffer.size());

    DoRead();
}

template <StringErrorCodeSessionCallbackConcept ReadCallback, ErrorCodeAndSessionCallbackConcept AcceptCallback,
          StringErrorCodeCallbackConcept WriteCallback>
void WebsocketServiceSession<ReadCallback, AcceptCallback, WriteCallback>::OnWrite(boost::system::error_code ec,
                                                                                   std::size_t)
{
    m_writeCallback(ec);
    if (ec)
    {
        return;
    }

    // Remove the string from the queue
    m_sendQueue.pop_front();

    // Send the next message if any
    if (!m_sendQueue.empty())
    {
        m_websocketStream.async_write(
                net::buffer(*m_sendQueue.front()),
                beast::bind_front_handler(&WebsocketServiceSession::OnWrite, this->shared_from_this()));
    }
}
} // namespace inklink::server_network
