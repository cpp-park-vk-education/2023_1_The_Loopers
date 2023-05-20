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
