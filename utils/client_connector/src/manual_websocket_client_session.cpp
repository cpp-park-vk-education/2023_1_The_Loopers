#include "manual_client_session.h"

#include <boost/posix_time.h>

namespace inklink::client_connector
{
namespace net = boost::asio;
namespace beast = boost::beast;
using error_code = boost::system::error_code;

template <Do_ConnectTypeErrorCode_Concept DoOnConnectType, Do_StringErrorCode_Concept DoOnRead,
          Do_ErrorCode_Concept DoOnWrite, Do_ErrorCode_Concept DoOnClose>
WebsocketClientSession<DoOnConnectType, DoOnRead, DoOnWrite, DoOnClose>::WebsocketClientSession(
        net::io_context& ioc, const std::string& address, unsigned short port, DoOnConnectType doOnConnectType,
        DoOnRead doOnRead, DoOnWrite doOnWrite, DoOnClose doOnClose)
        : m_timer{ioc}, m_doOnConnectType{doOnConnectType}, m_doOnRead{doOnRead}, m_doOnWrite{doOnWrite},
          m_doOnClose{doOnClose}
{
    net::ip::tcp::endpoint localEndpoint(boost::asio::ip::address::from_string(address), port);
    // do not realy need strand here, and I'm not sure can be socket constructed using strand
    m_socket = boost::asio::ip::tcp::socket(ioc, localEndpoint);
}

template <Do_ConnectTypeErrorCode_Concept DoOnConnectType, Do_StringErrorCode_Concept DoOnRead,
          Do_ErrorCode_Concept DoOnWrite, Do_ErrorCode_Concept DoOnClose>
void WebsocketClientSession<DoOnConnectType, DoOnRead, DoOnWrite, DoOnClose>::RunAsync(const std::string& host,
                                                                                       unsigned short port)
{
    m_host = host;
    m_host += ':' + std::to_string(port);

    // socket must be connected manually: asio::connect reset local endpoint bind
    net::ip::tcp::endpoint serverEndpoint(boost::asio::ip::address::from_string(host), port);
    error_code ec{};
    m_socket.async_connect(serverEndpoint, [this](error_code ec) { this->OnConnect(ec); });

    // in case of no connection with internet etc.
    m_timer.expires_from_now(boost::posix_time::seconds(30));
    // Set up the timer's expiration handler
    m_timer.async_wait(
            [&m_socket](const error_code& ec)
            {
                if (ec != net::error::operation_aborted)
                {
                    // Timeout occurred, cancel the socket's asynchronous operations
                    m_socket.cancel();
                    // now on connect will be called
                }
            });
}

template <Do_ConnectTypeErrorCode_Concept DoOnConnectType, Do_StringErrorCode_Concept DoOnRead,
          Do_ErrorCode_Concept DoOnWrite, Do_ErrorCode_Concept DoOnClose>
void WebsocketClientSession<DoOnConnectType, DoOnRead, DoOnWrite, DoOnClose>::Send(const std::string& msgBody)
{
    auto ss = std::make_shared<std::string const>(message);
    // Always add to queue
    m_queue.push_back(ss);

    // Are we already writing?
    if (m_queue.size() > 1)
        return;

    // We are not currently writing, so send this immediately
    m_ws.async_write(net::buffer(*m_queue.front()),
                     beast::bind_front_handler(&WebsocketClientSession::OnWrite, this->shared_from_this()));
}

template <Do_ConnectTypeErrorCode_Concept DoOnConnectType, Do_StringErrorCode_Concept DoOnRead,
          Do_ErrorCode_Concept DoOnWrite, Do_ErrorCode_Concept DoOnClose>
void WebsocketClientSession<DoOnConnectType, DoOnRead, DoOnWrite, DoOnClose>::Close()
{
    // TODO (a.novak) stop every read etc.

    m_ws.async_close(websocket::close_code::normal,
                     beast::bind_front_handler(&WebsocketClientSession::OnClose, this->shared_from_this()));
}

template <Do_ConnectTypeErrorCode_Concept DoOnConnectType, Do_StringErrorCode_Concept DoOnRead,
          Do_ErrorCode_Concept DoOnWrite, Do_ErrorCode_Concept DoOnClose>
void WebsocketClientSession<DoOnConnectType, DoOnRead, DoOnWrite, DoOnClose>::DoRead()
{
    // Read a message into our buffer
    m_ws.async_read(m_buffer, beast::bind_front_handler(&WebsocketClientSession::OnRead, this->shared_from_this()));
}

template <Do_ConnectTypeErrorCode_Concept DoOnConnectType, Do_StringErrorCode_Concept DoOnRead,
          Do_ErrorCode_Concept DoOnWrite, Do_ErrorCode_Concept DoOnClose>
void WebsocketClientSession<DoOnConnectType, DoOnRead, DoOnWrite, DoOnClose>::OnConnect(error_code ec)
{
    m_timer->cancel();
    m_doOnConnectType(ConnectType::kConnect, ec);
    if (ec)
    {
        return;
    }

    m_ws = websocket::stream<net::ip::tcp::socket>(std::move(m_socket));

    // Turn off the timeout on the tcp_stream, because
    // the websocket stream has its own timeout system.
    beast::get_lowest_layer(m_ws).expires_never();

    // Set suggested timeout settings for the websocket
    m_ws.set_option(websocket::stream_base::timeout::suggested(beast::role_type::client));

    // Set a decorator to change the User-Agent of the handshake
    m_ws.set_option(websocket::stream_base::decorator(
            [](websocket::request_type& req)
            { req.set(http::field::user_agent, std::string(BOOST_BEAST_VERSION_STRING) + " inklink-client"); }));

    // Perform the websocket handshake
    m_ws.async_handshake(m_host, "/",
                         beast::bind_front_handler(&WebsocketClientSession::OnHandshake, this->shared_from_this()));
}

template <Do_ConnectTypeErrorCode_Concept DoOnConnectType, Do_StringErrorCode_Concept DoOnRead,
          Do_ErrorCode_Concept DoOnWrite, Do_ErrorCode_Concept DoOnClose>
void WebsocketClientSession<DoOnConnectType, DoOnRead, DoOnWrite, DoOnClose>::OnHandshake(error_code ec)
{
    m_doOnConnectType(ConnectType::kHandshake, ec);
    if (ec)
    {
        return;
    }

    DoRead();
}

template <Do_ConnectTypeErrorCode_Concept DoOnConnectType, Do_StringErrorCode_Concept DoOnRead,
          Do_ErrorCode_Concept DoOnWrite, Do_ErrorCode_Concept DoOnClose>
void WebsocketClientSession<DoOnConnectType, DoOnRead, DoOnWrite, DoOnClose>::OnWrite(error_code ec, std::size_t)
{
    m_doOnWrite(ec);

    if (ec)
    {
        return;
    }

    // Remove the string from the queue
    m_queue.pop_front();

    // Send the next message if any
    if (!m_queue.empty())
    {
        m_ws.async_write(net::buffer(*m_queue.front()),
                         beast::bind_front_handler(&WebsocketClientSession::OnWrite, this->shared_from_this()));
    }
}

template <Do_ConnectTypeErrorCode_Concept DoOnConnectType, Do_StringErrorCode_Concept DoOnRead,
          Do_ErrorCode_Concept DoOnWrite, Do_ErrorCode_Concept DoOnClose>
void WebsocketClientSession<DoOnConnectType, DoOnRead, DoOnWrite, DoOnClose>::OnRead(error_code ec, std::size_t)
{
    m_doOnRead(beast::buffer::to_string(m_buffer.data()), ec);
    if (ec)
    {
        return;
    }

    m_buffer.consume(m_buffer.size());

    DoRead();
}

template <Do_ConnectTypeErrorCode_Concept DoOnConnectType, Do_StringErrorCode_Concept DoOnRead,
          Do_ErrorCode_Concept DoOnWrite, Do_ErrorCode_Concept DoOnClose>
void WebsocketClientSession<DoOnConnectType, DoOnRead, DoOnWrite, DoOnClose>::OnClose(error_code ec)
{
    m_doOnClose(ec);
    if (ec)
    {
        return;
    }
}
} // namespace inklink::client_connector
