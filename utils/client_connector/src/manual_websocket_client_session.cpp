#include "manual_websocket_client_session.h"

namespace
{
namespace net = boost::asio;
namespace beast = boost::beast;
namespace websocket = beast::websocket;
using error_code = boost::system::error_code;

constexpr char* kClientName = beast::BOOST_BEAST_VERSION_STRING " inklink-client";
} // namespace

namespace inklink::client_connector
{
// clang-format off
template <ConnectTypeErrorCodeCallbackConcept ConnectCallback, StringErrorCodeCallbackConcept ReadCallback,
          ErrorCodeCallbackConcept WriteCallback, ErrorCodeCallbackConcept CloseCallback>
ManualWebsocketClientSession<ConnectCallback, ReadCallback, WriteCallback, CloseCallback>::ManualWebsocketClientSession(
        net::io_context& ioc, 
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
    net::ip::tcp::endpoint localEndpoint(boost::asio::ip::address::from_string(address), port);
    // do not realy need strand here, and I'm not sure can be socket constructed using strand
    m_socket = boost::asio::ip::tcp::socket(ioc, localEndpoint);
}
// clang-format on

template <ConnectTypeErrorCodeCallbackConcept ConnectCallback, StringErrorCodeCallbackConcept ReadCallback,
          ErrorCodeCallbackConcept WriteCallback, ErrorCodeCallbackConcept CloseCallback>
ManualWebsocketClientSession<ConnectCallback, ReadCallback, WriteCallback,
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
        m_websocketStream->close(websocket::close_code::normal, ec);
        m_closeCallback(ec);
    }

    delete m_websocketStream;
    m_websocketStream = nullptr;
}

template <ConnectTypeErrorCodeCallbackConcept ConnectCallback, StringErrorCodeCallbackConcept ReadCallback,
          ErrorCodeCallbackConcept WriteCallback, ErrorCodeCallbackConcept CloseCallback>
void ManualWebsocketClientSession<ConnectCallback, ReadCallback, WriteCallback, CloseCallback>::RunAsync(
        const std::string& host, unsigned short port)
{
    m_host = host + ':' + std::to_string(port);

    // socket must be connected manually: asio::connect reset local endpoint bind
    net::ip::tcp::endpoint serverEndpoint(boost::asio::ip::address::from_string(host), port);
    m_socket.async_connect(serverEndpoint, [this](error_code errCode) { this->OnConnect(errCode); });

    // in case of no connection with internet etc.
    m_timer.expires_from_now(boost::posix_time::seconds(30));
    // Set up the timer's expiration handler
    m_timer.async_wait(
            [this](const error_code& errCode)
            {
                if (errCode != net::error::operation_aborted)
                {
                    // Timeout occurred, cancel the socket's asynchronous operations
                    m_socket.cancel();
                    // now on connect will be called
                }
            });
}

template <ConnectTypeErrorCodeCallbackConcept ConnectCallback, StringErrorCodeCallbackConcept ReadCallback,
          ErrorCodeCallbackConcept WriteCallback, ErrorCodeCallbackConcept CloseCallback>
void ManualWebsocketClientSession<ConnectCallback, ReadCallback, WriteCallback, CloseCallback>::Send(
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
            net::buffer(*m_sendQueue.front()),
            beast::bind_front_handler(&ManualWebsocketClientSession::OnWrite, this->shared_from_this()));
}

template <ConnectTypeErrorCodeCallbackConcept ConnectCallback, StringErrorCodeCallbackConcept ReadCallback,
          ErrorCodeCallbackConcept WriteCallback, ErrorCodeCallbackConcept CloseCallback>
void ManualWebsocketClientSession<ConnectCallback, ReadCallback, WriteCallback, CloseCallback>::Close()
{
    m_close = true;

    if (!m_writing.load())
    {
        m_websocketStream->async_close(
                websocket::close_code::normal,
                beast::bind_front_handler(&ManualWebsocketClientSession::OnClose, this->shared_from_this()));
    }
}

template <ConnectTypeErrorCodeCallbackConcept ConnectCallback, StringErrorCodeCallbackConcept ReadCallback,
          ErrorCodeCallbackConcept WriteCallback, ErrorCodeCallbackConcept CloseCallback>
void ManualWebsocketClientSession<ConnectCallback, ReadCallback, WriteCallback, CloseCallback>::DoRead()
{
    if (!m_close.load())
    {
        // Read a message into our buffer
        m_websocketStream->async_read(m_readBuffer, beast::bind_front_handler(&ManualWebsocketClientSession::OnRead,
                                                                              this->shared_from_this()));
    }
}

template <ConnectTypeErrorCodeCallbackConcept ConnectCallback, StringErrorCodeCallbackConcept ReadCallback,
          ErrorCodeCallbackConcept WriteCallback, ErrorCodeCallbackConcept CloseCallback>
void ManualWebsocketClientSession<ConnectCallback, ReadCallback, WriteCallback, CloseCallback>::OnConnect(error_code ec)
{
    m_timer.cancel();
    m_connectCallback(ConnectType::kConnect, ec);
    if (ec)
    {
        return;
    }

    m_websocketStream = new websocket::stream<beast::tcp_stream>(std::move(m_socket));

    // Turn off the timeout on the tcp_stream, because
    // the websocket stream has its own timeout system.
    beast::get_lowest_layer(*m_websocketStream).expires_never();

    // Set suggested timeout settings for the websocket
    m_websocketStream->set_option(websocket::stream_base::timeout::suggested(beast::role_type::client));

    // Set a decorator to change the User-Agent of the handshake
    m_websocketStream->set_option(websocket::stream_base::decorator(
            [](websocket::request_type& req) { req.set(beast::http::field::user_agent, kClientName); }));

    // Perform the websocket handshake
    m_websocketStream->async_handshake(
            m_host, "/",
            beast::bind_front_handler(&ManualWebsocketClientSession::OnHandshake, this->shared_from_this()));
}

template <ConnectTypeErrorCodeCallbackConcept ConnectCallback, StringErrorCodeCallbackConcept ReadCallback,
          ErrorCodeCallbackConcept WriteCallback, ErrorCodeCallbackConcept CloseCallback>
void ManualWebsocketClientSession<ConnectCallback, ReadCallback, WriteCallback, CloseCallback>::OnHandshake(
        error_code ec)
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
void ManualWebsocketClientSession<ConnectCallback, ReadCallback, WriteCallback, CloseCallback>::OnWrite(error_code ec,
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
                websocket::close_code::normal,
                beast::bind_front_handler(&ManualWebsocketClientSession::OnClose, this->shared_from_this()));
    }

    // Send the next message if any
    if (!m_sendQueue.empty())
    {
        m_websocketStream->async_write(
                net::buffer(*m_sendQueue.front()),
                beast::bind_front_handler(&ManualWebsocketClientSession::OnWrite, this->shared_from_this()));
    }
    else
    {
        m_writing = false;
    }
}

template <ConnectTypeErrorCodeCallbackConcept ConnectCallback, StringErrorCodeCallbackConcept ReadCallback,
          ErrorCodeCallbackConcept WriteCallback, ErrorCodeCallbackConcept CloseCallback>
void ManualWebsocketClientSession<ConnectCallback, ReadCallback, WriteCallback, CloseCallback>::OnRead(error_code ec,
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
void ManualWebsocketClientSession<ConnectCallback, ReadCallback, WriteCallback, CloseCallback>::OnClose(error_code ec)
{
    m_closeCallback(ec);
    if (ec)
    {
        return;
    }
}
} // namespace inklink::client_connector
