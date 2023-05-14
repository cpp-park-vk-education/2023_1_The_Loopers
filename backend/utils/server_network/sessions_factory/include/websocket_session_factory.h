#ifndef _WEBSOCKETSESSIONSFACTORY_H_
#define _WEBSOCKETSESSIONSFACTORY_H_

#include "isessions_factory.h"

#include <memory>
#include <string>

namespace inklink::server_network
{
template <typename T>
concept Do_ErrorCodeAndSession_Concept = requires(T&& t, boost::system::error_code ec, IServiceSession* session) {
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
    using IAuthorizer = authorizer::IAuthorizer;
    using InternalSessionsManager = base_service_chassis::InternalSessionsManager;
    using tcp = boost::asio::ip::tcp;

public:
    WebsocketSessionsFactory() = delete;

    explicit WebsocketSessionsFactory(
            std::shared_ptr<InternalSessionsManager>, std::shared_ptr<IAuthorizer>,
            DoOnRead = [](boost::system::error_code, IServiceSession*) {},
            DoOnAccept = [](boost::system::error_code, IServiceSession*) {},
            DoOnWrite = [](boost::system::error_code) {}) noexcept;

    WebsocketSessionsFactory(const WebsocketSessionsFactory&) noexcept = default;
    WebsocketSessionsFactory(WebsocketSessionsFactory&&) noexcept = default;

    WebsocketSessionsFactory& operator=(const WebsocketSessionsFactory&) noexcept = default;
    WebsocketSessionsFactory& operator=(WebsocketSessionsFactory&&) noexcept = default;

    ~WebsocketSessionsFactory() final = default;

    std::shared_ptr<IServiceSession> GetSession(boost::asio::ip::tcp::socket&&) final;

private:
    DoOnRead m_doOnRead;
    DoOnAccept m_doOnAccept;
    DoOnWrite m_doOnWrite;
};
} // namespace inklink::server_network

#endif // _WEBSOCKETSESSIONSFACTORY_H_
