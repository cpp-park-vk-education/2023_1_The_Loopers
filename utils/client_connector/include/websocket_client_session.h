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
namespace net = boost::asio;
namespace beast = boost::beast;

/**
 * @class WebsocketClientSession
 *
 * @brief Websocket session
 *
 * @tparam ConnectCallback callback with signature void( @ref ConnectType "ConnectType", boost::system::error_code,
 * IClientSession*). It's called on resolve, connect
 * @tparam ReadCallback callback with signature void(const std::string&, boost::system::error_code, IClientSession*).
 * It's called on new message from network
 * @tparam WriteCallback callback with signature void(const std::string&, boost::system::error_code, IClientSession*)
 * @tparam CloseCallback
 *
 * You can find more information about callbacks in the @ref ClientConcepts "client concepts" documentation
 *
 * @note Callbacks will be called from io_context thread!
 * @note It's recommended to read code example: there are some things needed to take into account for proper use
 *
 * @warning MUST be constructed using shared_ptr
 * @warning If you want to store session somewhere, you MUST store it as weak_ptr (preferred) or raw ptr. Under the hood
 * it calls every async operation with shared_ptr<WebsocketClientSession>(this) and when there is no more async
 * operations running it will destruct itself (for example, when connection is closed)
 * @warning You MUST NOT delete session if any error occurred! It will destruct itself automatically without your
 * intervention
 *
 * @see IClientSession
 *
 * @code {.cpp}
 * // Example code demonstrating the usage
 *
 * #include <websocket_client_session.h>
 *
 * #include <boost/asio.hpp>
 * #include <boost/system/error_code.hpp>
 *
 * #include <thread>
 * #include <functional> // if you are preferring std::bind
 *
 * using namespace inklink::client_connector;
 * using error_code = boost::system::error_code;
 *
 * class YourClass {
 *      std::weak_ptr<IClientSession> m_session;
 *
 *      boost::asio::io_context m_ioContext;
 *
 *      // to prevent ioContext from stopping on its own (when no more work to do)
 *      // if boost::asio version >= 1.79 (use with caution: for now it's very rare)
 *      // boost::asio::executor_work_guard<boost::asio::io_context::executor_type> m_ioContextExecutor;
 *      // version >= 1.74 (default on ubuntu 23; verified: may be older too)
 *      boost::asio::any_io_executor m_ioContextExecutor;
 *
 *      std::thread m_thread_ioContext;
 *
 * public:
 *      YourClass() {
 *              // I could not find the way to deduce template params automatically
 *              auto lamOnAccept = [this](ConnectType, error_code ec, IClientSession*) {
 *                      this->DoOnAccept(ec);
 *              };
 *              auto bindOnRead = std::bind(&YourClass::DoOnRead, this,
 *                                      std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
 *                                      // probably can skip _1 etc, if you need only error_code, but not 100% sure
 *              auto session = std::make_shared<WebsocketClientSession<
 *                                                                      decltype(lamOnAccept),
 *                                                                      decltype(bindOnRead)>>
 *                                                      (m_ioContext, lamOnAccept, bindOnRead);
 *              session->RunAsync("example.org", 80); // or ("127.0.0.1", 80)
 *              m_session = session;
 *
 *              // prevents ioContext from stopping before .stop(). If you want this only until smth happens
 *              // you can do following:
 *              // m_ioContextExecutor = boost::asio::any_io_executor(); (or call m_ioContextExecutor.reset() on
 *              // boost::asio >= 1.79)
 *              // (for example, until user goes to different tab: you do not want to stop ioContext but
 *              // in case of end of operations it's ok to stop automatically)
 *              // if boost::asio version >= 1.79
 *              // m_ioContextExecutor = boost::asio::make_work_guard(m_ioContext);
 *              // boost::asio version >= 1.74 (verified: may be older too)
 *              m_ioContextExecutor = boost::asio::require(m_ioContext.get_executor(),
 *                                                         boost::asio::execution::outstanding_work.tracked);
 *
 *              // running in separate thread, because io_context blocks thread where is running
 *              // One cane freely add sessions while io_context running: it is thread safe
 *              m_thread_ioContext = std::thread([this]() { this->m_ioContext.run(); });
 *
 *              // see https://www.boost.org/doc/libs/1_74_0/doc/html/boost_asio/reference/io_context.html or newer for
 *              // more information about io_context. Some notable things from there:
 *              // 1) exceptions will be propagated to thread where .run() was called. So you can write wrapper around
 *              // io context which catches exceptions and propagates to app as uou want.
 *              // 2) you can run anything in io_context if you want to: boost::asio::post(io_context, MyTask); => the
 *              // whole application too
 *      }
 *      ~YourClass() {
 *              m_ioContext.stop(); // stops io_context => all async operations in all sessions will end and
 *                                  // all sessions will be properly destroyed
 *              m_thread_ioContext.join(); // wait for thread end
 *      }
 *
 *      // remember, it will be called from different thread! (where running io_context)
 *      void DoOnAccept(error_code) {
 *              // your logic (log if (ec), for example)
 *              return;
 *      }
 *
 *      void DoOnRead(ConnectType, error_code ec, IClientSession*) {
 *              // your logic
 *              return;
 *      }
 * };
 *
 * @endcode
 */
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
            ConnectCallback = [](ConnectType, boost::system::error_code, IClientSession*) {},
            ReadCallback = [](const std::string&, boost::system::error_code, IClientSession*) {},
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

private:
    void DoRead();

    void OnResolve(error_code ec, net::ip::tcp::resolver::results_type results);
    void OnConnect(error_code ec, net::ip::tcp::resolver::results_type::endpoint_type ep);
    void OnHandshake(error_code ec);
    void OnWrite(error_code ec, std::size_t bytes_transferred);
    void OnRead(error_code ec, std::size_t bytes_transferred);
    void OnClose(error_code ec);

private:
    std::string m_host;
    net::ip::tcp::resolver m_resolver;
    boost::beast::websocket::stream<beast::tcp_stream> m_websocketStream;

    std::atomic_bool m_close{false};
    std::atomic_bool m_writing{false};

    beast::flat_buffer m_readBuffer;
    std::deque<std::shared_ptr<std::string const>> m_sendQueue;

    ConnectCallback m_connectCallback;
    ReadCallback m_readCallback;
    WriteCallback m_writeCallback;
    CloseCallback m_closeCallback;
};
} // namespace inklink::client_connector

namespace inklink::client_connector
{
// clang-format off
template <ConnectTypeErrorCodeCallbackConcept ConnectCallback, StringErrorCodeCallbackConcept ReadCallback,
          ErrorCodeCallbackConcept WriteCallback, ErrorCodeCallbackConcept CloseCallback> 
inline WebsocketClientSession<ConnectCallback, ReadCallback, WriteCallback, CloseCallback>::WebsocketClientSession(
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
inline void
WebsocketClientSession<ConnectCallback, ReadCallback, WriteCallback, CloseCallback>::RunAsync(const std::string& host,
                                                                                              unsigned short port)
{
    // Save these for later
    m_host = host;

    // Look up the domain name
    m_resolver.async_resolve(host, std::to_string(port),
                             beast::bind_front_handler(&WebsocketClientSession::OnResolve, this->shared_from_this()));
}

template <ConnectTypeErrorCodeCallbackConcept ConnectCallback, StringErrorCodeCallbackConcept ReadCallback,
          ErrorCodeCallbackConcept WriteCallback, ErrorCodeCallbackConcept CloseCallback>
inline WebsocketClientSession<ConnectCallback, ReadCallback, WriteCallback, CloseCallback>::~WebsocketClientSession()
{
    // TODO (a.novak) try-catch
    if (m_websocketStream.is_open())
    {
        // may throw due to another simultaneous write => just so that app won't crush
        // also does not stop async operations, therefore if isn't called because all operations ended other async
        // will throw.
        // to avoid last, before destruction of main app need must call io_context.stop() (to not think about order
        // of destruction of sessions and io_context)
        m_close = true;
        while (m_writing.load())
        {
            continue;
        }

        error_code ec;
        m_websocketStream.close(boost::beast::websocket::close_code::normal, ec);
        m_closeCallback(ec);
    }
}

template <ConnectTypeErrorCodeCallbackConcept ConnectCallback, StringErrorCodeCallbackConcept ReadCallback,
          ErrorCodeCallbackConcept WriteCallback, ErrorCodeCallbackConcept CloseCallback>
inline void
WebsocketClientSession<ConnectCallback, ReadCallback, WriteCallback, CloseCallback>::Send(const std::string& message)
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
inline void WebsocketClientSession<ConnectCallback, ReadCallback, WriteCallback, CloseCallback>::Close()
{
    m_close = true;

    if (!m_writing.load())
    {
        m_websocketStream.async_close(
                boost::beast::websocket::close_code::normal,
                beast::bind_front_handler(&WebsocketClientSession::OnClose, this->shared_from_this()));
    }
}

template <ConnectTypeErrorCodeCallbackConcept ConnectCallback, StringErrorCodeCallbackConcept ReadCallback,
          ErrorCodeCallbackConcept WriteCallback, ErrorCodeCallbackConcept CloseCallback>
inline void WebsocketClientSession<ConnectCallback, ReadCallback, WriteCallback, CloseCallback>::DoRead()
{
    if (!m_close.load())
    {
        m_websocketStream.async_read(
                m_readBuffer, beast::bind_front_handler(&WebsocketClientSession::OnRead, this->shared_from_this()));
    }
}

template <ConnectTypeErrorCodeCallbackConcept ConnectCallback, StringErrorCodeCallbackConcept ReadCallback,
          ErrorCodeCallbackConcept WriteCallback, ErrorCodeCallbackConcept CloseCallback>
inline void WebsocketClientSession<ConnectCallback, ReadCallback, WriteCallback, CloseCallback>::OnResolve(
        error_code ec, net::ip::tcp::resolver::results_type results)
{
    m_connectCallback(ConnectType::kResolve, ec, this);
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
inline void WebsocketClientSession<ConnectCallback, ReadCallback, WriteCallback, CloseCallback>::OnConnect(
        error_code ec, net::ip::tcp::resolver::results_type::endpoint_type ep)
{
    m_connectCallback(ConnectType::kConnect, ec, this);
    if (ec)
    {
        return;
    }

    // Turn off the timeout on the tcp_stream, because
    // the boost::beast::websocket stream has its own timeout system.
    beast::get_lowest_layer(m_websocketStream).expires_never();

    // Set suggested timeout settings for the boost::beast::websocket
    m_websocketStream.set_option(boost::beast::websocket::stream_base::timeout::suggested(beast::role_type::client));

    // Set a decorator to change the User-Agent of the handshake
    m_websocketStream.set_option(boost::beast::websocket::stream_base::decorator(
            [](boost::beast::websocket::request_type& req)
            { req.set(beast::http::field::user_agent, std::string(BOOST_BEAST_VERSION_STRING) + " inklink-client"); }));

    // Update the m_host string. This will provide the value of the
    // Host HTTP header during the WebSocket handshake.
    // See https://tools.ietf.org/html/rfc7230#section-5.4
    m_host += ':' + std::to_string(ep.port());

    // Perform the boost::beast::websocket handshake
    m_websocketStream.async_handshake(
            m_host, "/", beast::bind_front_handler(&WebsocketClientSession::OnHandshake, this->shared_from_this()));
}

template <ConnectTypeErrorCodeCallbackConcept ConnectCallback, StringErrorCodeCallbackConcept ReadCallback,
          ErrorCodeCallbackConcept WriteCallback, ErrorCodeCallbackConcept CloseCallback>
inline void
WebsocketClientSession<ConnectCallback, ReadCallback, WriteCallback, CloseCallback>::OnHandshake(error_code ec)
{
    m_connectCallback(ConnectType::kHandshake, ec, this);
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
inline void WebsocketClientSession<ConnectCallback, ReadCallback, WriteCallback, CloseCallback>::OnWrite(error_code ec,
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
                boost::beast::websocket::close_code::normal,
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
inline void WebsocketClientSession<ConnectCallback, ReadCallback, WriteCallback, CloseCallback>::OnRead(error_code ec,
                                                                                                        std::size_t)
{
    m_readCallback(beast::buffers_to_string(m_readBuffer.data()), ec, this);
    if (ec)
    {
        return;
    }

    m_readBuffer.consume(m_readBuffer.size());

    DoRead();
}

template <ConnectTypeErrorCodeCallbackConcept ConnectCallback, StringErrorCodeCallbackConcept ReadCallback,
          ErrorCodeCallbackConcept WriteCallback, ErrorCodeCallbackConcept CloseCallback>
inline void WebsocketClientSession<ConnectCallback, ReadCallback, WriteCallback, CloseCallback>::OnClose(error_code ec)
{
    m_closeCallback(ec);
    if (ec)
    {
        return;
    }
}
} // namespace inklink::client_connector
