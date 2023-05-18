#include "websocket_client_session.h"

namespace
{
namespace websocket = boost::beast::websocket;
using error_code = boost::system::error_code;
} // namespace

namespace inklink::client_connector
{
// clang-format off
template <ConnectTypeErrorCodeCallbackConcept ConnectCallback, StringErrorCodeCallbackConcept ReadCallback,
          ErrorCodeCallbackConcept WriteCallback, ErrorCodeCallbackConcept CloseCallback>
WebsocketClientSession<ConnectCallback, ReadCallback, WriteCallback, CloseCallback>::WebsocketClientSession(
        boost::asio::io_context& ioc, 
        ConnectCallback connectCallback, 
        ReadCallback readCallback,
        WriteCallback writeCallback, 
        CloseCallback closeCallback)
        : m_resolver{net::make_strand(ioc)}, 
          m_websocketStream{net::make_strand(ioc)},
          m_connectCallback{connectCallback}, 
          m_readCallback{readCallback}, 
          m_writeCallback{writeCallback},
          m_closeCallback{closeCallback}
{
}
// clang-format on

template <ConnectTypeErrorCodeCallbackConcept ConnectCallback, StringErrorCodeCallbackConcept ReadCallback,
          ErrorCodeCallbackConcept WriteCallback, ErrorCodeCallbackConcept CloseCallback>
void WebsocketClientSession<ConnectCallback, ReadCallback, WriteCallback, CloseCallback>::RunAsync(
        const std::string& host, unsigned short port)
{
    // Save these for later
    m_host = host;

    // Look up the domain name
    m_resolver.async_resolve(host, port,
                             beast::bind_front_handler(&WebsocketClientSession::OnResolve, this->shared_from_this()));
}

template <ConnectTypeErrorCodeCallbackConcept ConnectCallback, StringErrorCodeCallbackConcept ReadCallback,
          ErrorCodeCallbackConcept WriteCallback, ErrorCodeCallbackConcept CloseCallback>
WebsocketClientSession<ConnectCallback, ReadCallback, WriteCallback, CloseCallback>::~WebsocketClientSession()
{
    try
    {
        if (m_websocketStream.is_open())
        {
            // may throw due to another simultaneous write => just so that app won't crush
            // also does not stop async operations, therefore if isn't called because all operations ended other async
            // will throw.
            // to avoid last, before destruction of main app need must call io_context.stop() (to not think about order
            // of destruction of sessions and io_context)
            m_close = true;
            while (m_writing)
            {
                continue;
            }

            error_code ec;
            m_websocketStream.close(websocket::close_code::normal, ec);
            m_closeCallback(ec);
        }
    }
    catch (...)
    {
    }
}

template <ConnectTypeErrorCodeCallbackConcept ConnectCallback, StringErrorCodeCallbackConcept ReadCallback,
          ErrorCodeCallbackConcept WriteCallback, ErrorCodeCallbackConcept CloseCallback>
void WebsocketClientSession<ConnectCallback, ReadCallback, WriteCallback, CloseCallback>::Send(
        const std::string& message)
{
    auto ss = std::make_shared<std::string const>(message);
    // Always add to queue
    m_sendQueue.push_back(ss);
    m_writing = true;

    // Are we already writing?
    if (m_sendQueue.size() > 1)
    {
        return;
    }

    // We are not currently writing, so send this immediately
    m_websocketStream.async_write(
            net::buffer(*m_sendQueue.front()),
            beast::bind_front_handler(&WebsocketClientSession::OnWrite, this->shared_from_this()));
}

template <ConnectTypeErrorCodeCallbackConcept ConnectCallback, StringErrorCodeCallbackConcept ReadCallback,
          ErrorCodeCallbackConcept WriteCallback, ErrorCodeCallbackConcept CloseCallback>
void WebsocketClientSession<ConnectCallback, ReadCallback, WriteCallback, CloseCallback>::Close()
{
    m_close = true;

    if (!m_writing)
    {
        m_websocketStream.async_close(
                websocket::close_code::normal,
                beast::bind_front_handler(&WebsocketClientSession::OnClose, this->shared_from_this()));
    }
}

template <ConnectTypeErrorCodeCallbackConcept ConnectCallback, StringErrorCodeCallbackConcept ReadCallback,
          ErrorCodeCallbackConcept WriteCallback, ErrorCodeCallbackConcept CloseCallback>
void WebsocketClientSession<ConnectCallback, ReadCallback, WriteCallback, CloseCallback>::DoRead()
{
    if (!m_close)
    {
        m_websocketStream.async_read(
                m_readBuffer, beast::bind_front_handler(&WebsocketClientSession::OnRead, this->shared_from_this()));
    }
}

template <ConnectTypeErrorCodeCallbackConcept ConnectCallback, StringErrorCodeCallbackConcept ReadCallback,
          ErrorCodeCallbackConcept WriteCallback, ErrorCodeCallbackConcept CloseCallback>
void WebsocketClientSession<ConnectCallback, ReadCallback, WriteCallback, CloseCallback>::OnResolve(
        error_code ec, net::ip::tcp::resolver::results_type results)
{
    m_connectCallback(ConnectType::kResolve, ec);
    if (ec)
    {
        return;
    }

    // Set the timeout for the operation
    beast::get_lowest_layer(m_websocketStream).expires_after(std::chrono::seconds(30));

    // Make the connection on the IP address we get from a lookup
    beast::get_lowest_layer(m_websocketStream)
            .async_connect(results,
                           beast::bind_front_handler(&WebsocketClientSession::OnConnect, this->shared_from_this()));
}

template <ConnectTypeErrorCodeCallbackConcept ConnectCallback, StringErrorCodeCallbackConcept ReadCallback,
          ErrorCodeCallbackConcept WriteCallback, ErrorCodeCallbackConcept CloseCallback>
void WebsocketClientSession<ConnectCallback, ReadCallback, WriteCallback, CloseCallback>::OnConnect(
        error_code ec, net::ip::tcp::resolver::results_type::endpoint_type ep)
{
    m_connectCallback(ConnectType::kConnect, ec);
    if (ec)
    {
        return;
    }

    // Turn off the timeout on the tcp_stream, because
    // the websocket stream has its own timeout system.
    beast::get_lowest_layer(m_websocketStream).expires_never();

    // Set suggested timeout settings for the websocket
    m_websocketStream.set_option(websocket::stream_base::timeout::suggested(beast::role_type::client));

    // Set a decorator to change the User-Agent of the handshake
    m_websocketStream.set_option(websocket::stream_base::decorator(
            [](websocket::request_type& req)
            { req.set(beast::http::field::user_agent, std::string(BOOST_BEAST_VERSION_STRING) + " inklink-client"); }));

    // Update the m_host string. This will provide the value of the
    // Host HTTP header during the WebSocket handshake.
    // See https://tools.ietf.org/html/rfc7230#section-5.4
    m_host += ':' + std::to_string(ep.port());

    // Perform the websocket handshake
    m_websocketStream.async_handshake(
            m_host, "/", beast::bind_front_handler(&WebsocketClientSession::OnHandshake, this->shared_from_this()));
}

template <ConnectTypeErrorCodeCallbackConcept ConnectCallback, StringErrorCodeCallbackConcept ReadCallback,
          ErrorCodeCallbackConcept WriteCallback, ErrorCodeCallbackConcept CloseCallback>
void WebsocketClientSession<ConnectCallback, ReadCallback, WriteCallback, CloseCallback>::OnHandshake(error_code ec)
{
    m_connectCallback(ConnectType::kHandshake, ec);
    if (ec)
    {
        return;
    }

    // set to binary (because we don't know in wich format will be sending)
    m_websocketStream.binary(true);

    DoRead();
}

template <ConnectTypeErrorCodeCallbackConcept ConnectCallback, StringErrorCodeCallbackConcept ReadCallback,
          ErrorCodeCallbackConcept WriteCallback, ErrorCodeCallbackConcept CloseCallback>
void WebsocketClientSession<ConnectCallback, ReadCallback, WriteCallback, CloseCallback>::OnWrite(error_code ec,
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
        m_websocketStream.async_close(
                websocket::close_code::normal,
                beast::bind_front_handler(&WebsocketClientSession::OnClose, this->shared_from_this()));
    }

    // Send the next message if any
    if (!m_sendQueue.empty())
    {
        m_websocketStream.async_write(
                net::buffer(*m_sendQueue.front()),
                beast::bind_front_handler(&WebsocketClientSession::OnWrite, this->shared_from_this()));
    }
    else
    {
        m_writing = false;
    }
}

template <ConnectTypeErrorCodeCallbackConcept ConnectCallback, StringErrorCodeCallbackConcept ReadCallback,
          ErrorCodeCallbackConcept WriteCallback, ErrorCodeCallbackConcept CloseCallback>
void WebsocketClientSession<ConnectCallback, ReadCallback, WriteCallback, CloseCallback>::OnRead(error_code ec,
                                                                                                 std::size_t)
{
    m_readCallback(beast::buffers_to_string(m_readBuffer.data()), ec);
    if (ec)
    {
        return;
    }

    m_readBuffer.consume(m_readBuffer.size());

    DoRead();
}

template <ConnectTypeErrorCodeCallbackConcept ConnectCallback, StringErrorCodeCallbackConcept ReadCallback,
          ErrorCodeCallbackConcept WriteCallback, ErrorCodeCallbackConcept CloseCallback>
void WebsocketClientSession<ConnectCallback, ReadCallback, WriteCallback, CloseCallback>::OnClose(error_code ec)
{
    m_closeCallback(ec);
    if (ec)
    {
        return;
    }
}
} // namespace inklink::client_connector
