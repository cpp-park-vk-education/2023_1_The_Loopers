#include "websocket_client_session.h"

namespace inklink_client_session
{
namespace net = boost::asio;
namespace beast = boost::beast;
using error_code = boost::system::error_code;

template <Do_ConnectTypeErrorCode_Concept DoOnConnectType = Do_ConnectTypeErrorCode_Concept,
          Do_StringErrorCode_Concept DoOnRead = Fun_StringErrorCodeSession,
          Do_ErrorCode_Concept DoOnWrite = Fun_ErrorCode,
          Do_ErrorCode_Concept DoOnClose = Fun_ErrorCode>
WebsocketClientSession<DoOnConnectType, DoOnRead, DoOnWrite, DoOnClose>::WebsocketClientSession(
        boost::asio::io_context& ioc, DoOnConnectType doOnConnectType, DoOnRead doOnRead,
        DoOnWrite doOnWrite, DoOnClose doOnClose)
        : m_resolver(net::make_strand(ioc)), m_ws(net::make_strand(ioc)),
          m_doOnConnectType{doOnConnectType}, m_doOnRead{doOnRead}, m_doOnWrite{doOnWrite},
          m_doOnClose{doOnClose}
{
}

template <Do_ConnectTypeErrorCode_Concept DoOnConnectType = Do_ConnectTypeErrorCode_Concept,
          Do_StringErrorCode_Concept DoOnRead = Fun_StringErrorCodeSession,
          Do_ErrorCode_Concept DoOnWrite = Fun_ErrorCode,
          Do_ErrorCode_Concept DoOnClose = Fun_ErrorCode>
void WebsocketClientSession<DoOnConnectType, DoOnRead, DoOnWrite, DoOnClose>::RunAsync(
        const std::string& host, unsigned short port)
{
    // Save these for later
    m_host = host;

    // Look up the domain name
    m_resolver.async_resolve(
            host, port,
            beast::bind_front_handler(&WebsocketClientSession::OnResolve, shared_from_this()));
}

template <Do_ConnectTypeErrorCode_Concept DoOnConnectType = Do_ConnectTypeErrorCode_Concept,
          Do_StringErrorCode_Concept DoOnRead = Fun_StringErrorCodeSession,
          Do_ErrorCode_Concept DoOnWrite = Fun_ErrorCode,
          Do_ErrorCode_Concept DoOnClose = Fun_ErrorCode>
void WebsocketClientSession<DoOnConnectType, DoOnRead, DoOnWrite, DoOnClose>::Send(
        const std::string& msgBody)
{
    auto ss = std::make_shared<std::string const>(message);
    // Always add to queue
    m_queue.push_back(ss);

    // Are we already writing?
    if (m_queue.size() > 1)
        return;

    // We are not currently writing, so send this immediately
    m_bufferSending = net::buffer(*m_queue.front());

    m_ws.async_write(m_bufferSending, beast::bind_front_handler(&WebsocketClientSession::OnWrite,
                                                                shared_from_this()));
}

template <Do_ConnectTypeErrorCode_Concept DoOnConnectType = Do_ConnectTypeErrorCode_Concept,
          Do_StringErrorCode_Concept DoOnRead = Fun_StringErrorCodeSession,
          Do_ErrorCode_Concept DoOnWrite = Fun_ErrorCode,
          Do_ErrorCode_Concept DoOnClose = Fun_ErrorCode>
void WebsocketClientSession<DoOnConnectType, DoOnRead, DoOnWrite, DoOnClose>::Close()
{
    // TODO stop every read etc.

    m_ws.async_close(
            websocket::close_code::normal,
            beast::bind_front_handler(&WebsocketClientSession::OnClose, shared_from_this()));
}

template <Do_ConnectTypeErrorCode_Concept DoOnConnectType = Do_ConnectTypeErrorCode_Concept,
          Do_StringErrorCode_Concept DoOnRead = Fun_StringErrorCodeSession,
          Do_ErrorCode_Concept DoOnWrite = Fun_ErrorCode,
          Do_ErrorCode_Concept DoOnClose = Fun_ErrorCode>
void WebsocketClientSession<DoOnConnectType, DoOnRead, DoOnWrite, DoOnClose>::OnResolve(
        beast::error_code ec, tcp::resolver::results_type results)
{
    m_doOnConnectType(ConnectType::kResolve, ec);
    if (ec)
        return;

    // Set the timeout for the operation
    beast::get_lowest_layer(m_ws).expires_after(std::chrono::seconds(30));

    // Make the connection on the IP address we get from a lookup
    beast::get_lowest_layer(m_ws).async_connect(
            results,
            beast::bind_front_handler(&WebsocketClientSession::OnConnect, shared_from_this()));
}

template <Do_ConnectTypeErrorCode_Concept DoOnConnectType = Do_ConnectTypeErrorCode_Concept,
          Do_StringErrorCode_Concept DoOnRead = Fun_StringErrorCodeSession,
          Do_ErrorCode_Concept DoOnWrite = Fun_ErrorCode,
          Do_ErrorCode_Concept DoOnClose = Fun_ErrorCode>
void WebsocketClientSession<DoOnConnectType, DoOnRead, DoOnWrite, DoOnClose>::OnConnect(
        beast::error_code ec, tcp::resolver::results_type::endpoint_type ep)
{
    m_doOnConnectType(ConnectType::kConnect, ec);
    if (ec)
        return;

    // Turn off the timeout on the tcp_stream, because
    // the websocket stream has its own timeout system.
    beast::get_lowest_layer(m_ws).expires_never();

    // Set suggested timeout settings for the websocket
    m_ws.set_option(websocket::stream_base::timeout::suggested(beast::role_type::client));

    // Set a decorator to change the User-Agent of the handshake
    m_ws.set_option(websocket::stream_base::decorator(
            [](websocket::request_type& req)
            {
                req.set(http::field::user_agent,
                        std::string(BOOST_BEAST_VERSION_STRING) + " inklink-client");
            }));

    // Update the m_host string. This will provide the value of the
    // Host HTTP header during the WebSocket handshake.
    // See https://tools.ietf.org/html/rfc7230#section-5.4
    m_host += ':' + std::to_string(ep.port());

    // Perform the websocket handshake
    m_ws.async_handshake(
            m_host, "/",
            beast::bind_front_handler(&WebsocketClientSession::OnHandshake, shared_from_this()));
}

template <Do_ConnectTypeErrorCode_Concept DoOnConnectType = Do_ConnectTypeErrorCode_Concept,
          Do_StringErrorCode_Concept DoOnRead = Fun_StringErrorCodeSession,
          Do_ErrorCode_Concept DoOnWrite = Fun_ErrorCode,
          Do_ErrorCode_Concept DoOnClose = Fun_ErrorCode>
void WebsocketClientSession<DoOnConnectType, DoOnRead, DoOnWrite, DoOnClose>::OnHandshake(
        beast::error_code ec)
{
    m_doOnConnectType(ConnectType::kHandshake, ec);
    if (ec)
        return;

    // Read a message into our buffer
    m_ws.async_read(m_buffer,
                    beast::bind_front_handler(&WebsocketClientSession::OnRead, shared_from_this()));
}

template <Do_ConnectTypeErrorCode_Concept DoOnConnectType = Do_ConnectTypeErrorCode_Concept,
          Do_StringErrorCode_Concept DoOnRead = Fun_StringErrorCodeSession,
          Do_ErrorCode_Concept DoOnWrite = Fun_ErrorCode,
          Do_ErrorCode_Concept DoOnClose = Fun_ErrorCode>
void WebsocketClientSession<DoOnConnectType, DoOnRead, DoOnWrite, DoOnClose>::OnWrite(
        beast::error_code ec, std::size_t)
{
    m_doOnWrite(ec);

    if (ec)
        return;

    // Remove the string from the queue
    m_queue.pop_front();

    // Send the next message if any
    if (!m_queue.empty())
    {
        m_bufferSending = net::buffer(*m_queue.front());
        m_ws.async_write(
                m_bufferSending,
                beast::bind_front_handler(&WebsocketClientSession::OnWrite, shared_from_this()));
    }
}

template <Do_ConnectTypeErrorCode_Concept DoOnConnectType = Do_ConnectTypeErrorCode_Concept,
          Do_StringErrorCode_Concept DoOnRead = Fun_StringErrorCodeSession,
          Do_ErrorCode_Concept DoOnWrite = Fun_ErrorCode,
          Do_ErrorCode_Concept DoOnClose = Fun_ErrorCode>
void WebsocketClientSession<DoOnConnectType, DoOnRead, DoOnWrite, DoOnClose>::OnRead(
        beast::error_code ec, std::size_t)
{
    m_doOnRead(beast::buffer::to_string(m_buffer.data()), ec);
    if (ec)
        return;

    m_buffer.consume(m_buffer.size());

    DoRead();
}

template <Do_ConnectTypeErrorCode_Concept DoOnConnectType = Do_ConnectTypeErrorCode_Concept,
          Do_StringErrorCode_Concept DoOnRead = Fun_StringErrorCodeSession,
          Do_ErrorCode_Concept DoOnWrite = Fun_ErrorCode,
          Do_ErrorCode_Concept DoOnClose = Fun_ErrorCode>
void WebsocketClientSession<DoOnConnectType, DoOnRead, DoOnWrite, DoOnClose>::OnClose(
        beast::error_code ec)
{
    m_doOnClose(ec);
    if (ec)
        return;
}
}  // namespace inklink_client_session
