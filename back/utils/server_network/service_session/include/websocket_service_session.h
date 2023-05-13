#ifndef _WEBSOCKETSERVICESESSION_H_
#define _WEBSOCKETSERVICESESSION_H_

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/system/error_code.hpp>
#include <deque>
#include <memory>
#include <string>

#include "iservice_session.h"

namespace inklink_service_session
{
namespace net = boost::asio;
namespace beast = boost::beast;
namespace websocket = beast::websocket;

template <typename T>
concept Do_ErrorCodeAndSession_Concept =
        requires(T&& t, boost::system::error_code ec, IServiceSession* session) {
            {
                std::forward<T>(t)(ec, session)
            } -> std::same_as<void>;
        };

template <typename T>
concept Do_ErrorCode_Concept = requires(T&& t, boost::system::error_code ec) {
    {
        std::forward<T>(t)(ec)
    } -> std::same_as<void>;
};

using Fun_ErrorCodeAndSession = std::function<void(boost::system::error_code, IServiceSession*)>;
using Fun_ErrorCode = std::function<void(boost::system::error_code)>;

template <Do_ErrorCodeAndSession_Concept DoOnRead = Fun_ErrorCodeAndSession,
          Do_ErrorCodeAndSession_Concept DoOnAccept = Fun_ErrorCodeAndSession,
          Do_ErrorCode_Concept DoOnWrite = Fun_ErrorCode>
class WebsocketServiceSession : public IServiceSession,
                                public std::enable_shared_from_this<WebsocketServiceSession>
{
public:
    using error_code = boost::system::error_code;
    WebsocketServiceSession(
            net::ip::tcp::socket&&, DoOnRead = [](error_code, IServiceSession*) {},
            DoOnAccept = [](error_code, IServiceSession*) {}, DoOnWrite = [](error_code) {});

    WebsocketServiceSession() = delete;

    WebsocketServiceSession(const WebsocketServiceSession&) = delete;
    WebsocketServiceSession(WebsocketServiceSession&&) = delete;

    WebsocketServiceSession& operator=(const WebsocketServiceSession&) = delete;
    WebsocketServiceSession& operator=(WebsocketServiceSession&&) = delete;

    ~WebsocketServiceSession();

    virtual void run_async()
    {
    }
    virtual void send(const std::string&)
    {
    }

private:
    void OnAccept(boost::system::error_code ec);

    void DoRead();

    void OnRead(boost::system::error_code ec, std::size_t bytes_transferred);
    void OnWrite(boost::system::error_code ec, std::size_t bytes_transferred);

    websocket::stream<tcp::socket> m_ws;
    boost::beast::multi_buffer m_buffer;
    std::deque<std::shared_ptr<std::string const>> m_queue;
    
    DoOnRead m_doOnRead;
    DoOnAccept m_doOnAccept;
    DoOnWrite m_doOnWrite;
};
}  // namespace inklink_service_session

#endif  // _WEBSOCKETSERVICESESSION_H_
