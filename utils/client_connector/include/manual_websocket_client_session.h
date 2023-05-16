#ifndef _MANUAL_WEBSOCKET_CLIENT_SESSION_H_
#define _MANUAL_WEBSOCKET_CLIENT_SESSION_H_

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

template <Do_ConnectTypeErrorCode_Concept DoOnConnectType = Fun_ConnectTypeErrorCodeSession,
          Do_StringErrorCode_Concept DoOnRead = Fun_StringErrorCodeSession,
          Do_ErrorCode_Concept DoOnWrite = Fun_ErrorCode, Do_ErrorCode_Concept DoOnClose = Fun_ErrorCode>
class ManualWebsocketClientSession final
        : public IClientSession,
          public std::enable_shared_from_this<
                  ManualWebsocketClientSession<DoOnConnectType, DoOnRead, DoOnWrite, DoOnClose>>
{
public:
    ManualWebsocketClientSession() = delete;

    explicit ManualWebsocketClientSession(
            boost::asio::io_context&, const std::string& address, unsigned short port,
            DoOnConnectType = [](ConnectType ct, boost::system::error_code) {},
            DoOnRead = [](const std::string&, boost::system::error_code) {},
            DoOnWrite = [](boost::system::error_code) {}, DoOnClose = [](boost::system::error_code) {});

    ManualWebsocketClientSession(const ManualWebsocketClientSession&) = delete;
    ManualWebsocketClientSession(ManualWebsocketClientSession&&) = delete;

    ManualWebsocketClientSession& operator=(const ManualWebsocketClientSession&) = delete;
    ManualWebsocketClientSession& operator=(ManualWebsocketClientSession&&) = delete;

    ~ManualWebsocketClientSession() final;

    void RunAsync(const std::string& host, unsigned short port) final;
    void Send(const std::string&) final;
    void Close() final;

private:
    using error_code = boost::system::error_code;

    void DoRead();

    void OnConnect(error_code ec);
    void OnHandshake(error_code ec);
    void OnWrite(error_code ec, std::size_t bytes_transferred);
    void OnRead(error_code ec, std::size_t bytes_transferred);
    void OnClose(error_code ec);

    boost::asio::ip::tcp::socket m_socket;
    boost::asio::deadline_timer m_timer;

    bool m_close{false};
    bool m_writing{false};

    std::string m_host;
    beast::websocket::stream<beast::tcp_stream> m_ws;

    beast::flat_buffer m_buffer;
    std::deque<std::shared_ptr<std::string const>> m_queue;

    DoOnConnectType m_doOnConnectType;
    DoOnRead m_doOnRead;
    DoOnWrite m_doOnWrite;
    DoOnClose m_doOnClose;
};
} // namespace inklink::client_connector

#endif // _MANUAL_WEBSOCKET_CLIENT_SESSION_H_
