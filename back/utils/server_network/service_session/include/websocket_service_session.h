#ifndef _WEBSOCKETSERVICESESSION_H_
#define _WEBSOCKETSERVICESESSION_H_

#include <boost/system/error_code.hpp>
#include <memory>
#include <string>

#include "iservice_session.h"

namespace inklink_service_session
{
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
    WebsocketServiceSession() = delete;

    WebsocketServiceSession(const WebsocketServiceSession&) = delete;
    WebsocketServiceSession(WebsocketServiceSession&&) = delete;

    WebsocketServiceSession& operator=(const WebsocketServiceSession&) = delete;
    WebsocketServiceSession& operator=(WebsocketServiceSession&&) = delete;

    ~WebsocketServiceSession();

    virtual void run()
    {
    }
    virtual void run_async()
    {
    }
    virtual void send(const std::string&)
    {
    }

private:
    void OnRead();
};
}  // namespace inklink_service_session

#endif  // _WEBSOCKETSERVICESESSION_H_
