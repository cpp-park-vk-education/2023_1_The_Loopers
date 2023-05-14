#ifndef _IWEBSOCKETCLIENTSESSION_H_
#define _IWEBSOCKETCLIENTSESSION_H_

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/system/error_code.hpp>
#include <deque>
#include <functional>
#include <memory>
#include <string>

#include "iclient_session.h"

namespace inklink_client_session
{
enum ConnectType
{
    kResolve,
    kConnect,
    kHandshake
};

template <typename T>
concept Do_StringErrorCode_Concept = requires(
        T&& t, const std::string& msgBody, boost::system::error_code ec, IClientSession* session) {
    {
        std::forward<T>(t)(msgBody, ec, session)
    } -> std::same_as<void>;
};

template <typename T>
concept Do_ConnectTypeErrorCode_Concept =
        requires(T&& t, ConnectType ct, boost::system::error_code ec) {
            {
                std::forward<T>(t)(ct, ec)
            } -> std::same_as<void>;
        };

template <typename T>
concept Do_ErrorCode_Concept = requires(T&& t, boost::system::error_code ec) {
    {
        std::forward<T>(t)(ec)
    } -> std::same_as<void>;
};

using Fun_StringErrorCodeSession =
        std::function<void(const std::string&, boost::system::error_code, IClientSession*)>;
using Fun_ErrorCode = std::function<void(boost::system::error_code)>;

template <Do_ConnectTypeErrorCode_Concept DoOnConnectType = Do_ConnectTypeErrorCode_Concept,
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

    void OnResolve(beast::error_code ec, net::ip::tcp::resolver::results_type results);

    void OnConnect(beast::error_code ec, net::ip::tcp::resolver::results_type::endpoint_type ep);
    void OnHandshake(beast::error_code ec);
    void OnWrite(beast::error_code ec, std::size_t bytes_transferred);
    void OnRead(beast::error_code ec, std::size_t bytes_transferred);
    void OnClose(beast::error_code ec);

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

#endif  // _IWEBSOCKETCLIENTSESSION_H_
