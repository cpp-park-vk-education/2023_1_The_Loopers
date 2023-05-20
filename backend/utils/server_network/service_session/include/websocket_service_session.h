#pragma once

#include "inklink/server_network/callback_concepts.h"
#include "iservice_session.h"

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/system/error_code.hpp>

#include <concepts>
#include <deque>
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
          ErrorCodeCallbackConcept WriteCallback = std::function<void(boost::system::error_code)>>
class WebsocketServiceSession
        : public IServiceSession,
          public std::enable_shared_from_this<WebsocketServiceSession<ReadCallback, AcceptCallback, WriteCallback>>
{
    using IAuthorizer = authorizer::IAuthorizer;
    using InternalSessionsManager = base_service_chassis::InternalSessionsManager;
    using tcp = net::ip::tcp;

public:
    using error_code = boost::system::error_code;
    // clang-format off
    explicit WebsocketServiceSession(
            std::shared_ptr<InternalSessionsManager>, 
            std::shared_ptr<IAuthorizer>, 
            net::ip::tcp::socket&&,
            ReadCallback = [](const std::string&, error_code, IServiceSession*) {},
            AcceptCallback = [](error_code, IServiceSession*) {}, 
            WriteCallback = [](error_code) {}) noexcept;
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
