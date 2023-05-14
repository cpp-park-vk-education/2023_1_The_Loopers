#ifndef _WEBSOCKET_CLIENT_SESSION_H_
#define _WEBSOCKET_CLIENT_SESSION_H_

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/system/error_code.hpp>
#include <deque>
#include <functional>
#include <memory>
#include <string>

#include "global_websocket_client_session.h"
#include "iclient_session.h"

namespace inklink_client_session
{
template <Do_ConnectTypeErrorCode_Concept DoOnConnectType = Fun_ConnectTypeErrorCodeSession,
          Do_StringErrorCode_Concept DoOnRead = Fun_StringErrorCodeSession,
          Do_ErrorCode_Concept DoOnWrite = Fun_ErrorCode,
          Do_ErrorCode_Concept DoOnClose = Fun_ErrorCode>
class WebsocketClientSession : public IClientSession,
                               public std::enable_shared_from_this<WebsocketClientSession>
{
public:
    WebsocketClientSession() = delete;

    explicit WebsocketClientSession(
            boost::asio::io_context&,
            DoOnConnectType = [](ConnectType ct, boost::system::error_code) {},
            DoOnRead = [](const std::string&, boost::system::error_code) {},
            DoOnWrite = [](boost::system::error_code) {},
            DoOnClose = [](boost::system::error_code) {});

    WebsocketClientSession(const WebsocketClientSession&) = delete;
    WebsocketClientSession(WebsocketClientSession&&) = delete;

    WebsocketClientSession& operator=(const WebsocketClientSession&) = delete;
    WebsocketClientSession& operator=(WebsocketClientSession&&) = delete;

    ~WebsocketClientSession() = default;

    void RunAsync(const std::string& host, unsigned short port) override;
    void Send(const std::string&) override;
    void Close() override;

private:
    namespace net = boost::asio;
    namespace beast = boost::beast;
    using error_code = boost::system::error_code;

    void OnResolve(error_code ec, net::ip::tcp::resolver::results_type results);

    void OnConnect(error_code ec, net::ip::tcp::resolver::results_type::endpoint_type ep);
    void OnHandshake(error_code ec);
    void OnWrite(error_code ec, std::size_t bytes_transferred);
    void OnRead(error_code ec, std::size_t bytes_transferred);
    void OnClose(error_code ec);

    std::string m_host;
    net::ip::tcp::resolver m_resolver;
    beast::websocket::stream<beast::tcp_stream> m_ws;

    beast::flat_buffer m_buffer;
    std::deque<std::shared_ptr<std::string const>> m_queue;
    boost::asio::buffer m_bufferSending;

    DoOnConnectType m_doOnConnectType;
    DoOnRead m_doOnRead;
    DoOnWrite m_doOnWrite;
    DoOnClose m_doOnClose;
};
}  // namespace inklink_client_session

#endif  // _WEBSOCKET_CLIENT_SESSION_H_
