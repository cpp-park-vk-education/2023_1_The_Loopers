#ifndef _WEBSOCKETSESSIONSFACTORY_H_
#define _WEBSOCKETSESSIONSFACTORY_H_

#include <memory>
#include <string>

#include "isessions_factory.h"

namespace inklink_sessions_factory
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
class WebsocketSessionsFactory : public ISessionsFactory
{
public:
    WebsocketSessionsFactory() = delete;

    WebsocketSessionsFactory(
            DoOnRead = [](boost::system::error_code, IServiceSession*) {},
            DoOnAccept = [](boost::system::error_code, IServiceSession*) {},
            DoOnWrite = [](boost::system::error_code) {}) noexcept;

    WebsocketSessionsFactory(const WebsocketSessionsFactory&) = default noexcept;
    WebsocketSessionsFactory(WebsocketSessionsFactory&&) = default noexcept;

    WebsocketSessionsFactory& WebsocketSessionsFactory operator=(const WebsocketSessionsFactory&) =
            default;
    WebsocketSessionsFactory& WebsocketSessionsFactory operator=(WebsocketSessionsFactory&&) =
            default;

    ~WebsocketSessionsFactory() = default;

    std::shared_ptr<IServiceSession> GetSession(boost::asio::ip::tcp::socket&&) override;

private:
    DoOnRead m_doOnRead;
    DoOnAccept m_doOnAccept;
    DoOnWrite m_doOnWrite;
};
}  // namespace inklink_sessions_factory

#endif  // _WEBSOCKETSESSIONSFACTORY_H_
