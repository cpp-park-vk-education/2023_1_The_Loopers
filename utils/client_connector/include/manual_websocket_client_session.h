#ifndef _MANUAL_WEBSOCKET_CLIENT_SESSION_H_
#define _MANUAL_WEBSOCKET_CLIENT_SESSION_H_

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
class ManualWebsocketClientSession
        : public IClientSession,
          public std::enable_shared_from_this<ManualWebsocketClientSession>
{
public:
    ManualWebsocketClientSession() = delete;

    explicit ManualWebsocketClientSession(
            boost::asio::io_context&, const std::string& address, unsigned short port,
            DoOnConnectType = [](ConnectType ct, boost::system::error_code) {},
            DoOnRead = [](const std::string&, boost::system::error_code) {},
            DoOnWrite = [](boost::system::error_code) {},
            DoOnClose = [](boost::system::error_code) {});

    ManualWebsocketClientSession(const ManualWebsocketClientSession&) = delete;
    ManualWebsocketClientSession(ManualWebsocketClientSession&&) = delete;

    ManualWebsocketClientSession& operator=(const ManualWebsocketClientSession&) = delete;
    ManualWebsocketClientSession& operator=(ManualWebsocketClientSession&&) = delete;

    ~ManualWebsocketClientSession() = default;

    void RunAsync(const std::string& host, unsigned short port) override;
    void Send(const std::string&) override;
    void Close() override;

private:
    namespace net = boost::asio;
    namespace beast = boost::beast;
    using error_code = boost::system::error_code;

    void OnConnect(error_code ec);

    void OnHandshake(error_code ec);
    void OnWrite(error_code ec, std::size_t bytes_transferred);
    void OnRead(error_code ec, std::size_t bytes_transferred);
    void OnClose(error_code ec);

    boost::asio::ip::tcp::socket m_socket;
    boost::asio::deadline_timer m_timer;

    const std::string& m_host;
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

#endif  // _MANUAL_WEBSOCKET_CLIENT_SESSION_H_
