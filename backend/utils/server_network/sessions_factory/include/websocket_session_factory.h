#pragma once

#include "inklink/server_network/callback_concepts.h"
#include "isessions_factory.h"
#include "websocket_service_session.h"

#include <concepts>
#include <functional>
#include <memory>
#include <string>

namespace inklink::server_network
{

template <StringErrorCodeSessionCallbackConcept ReadCallback =
                  std::function<void(const std::string&, boost::system::error_code, IServiceSession*)>,
          ErrorCodeAndSessionCallbackConcept AcceptCallback =
                  std::function<void(boost::system::error_code, IServiceSession*)>,
          ErrorCodeCallbackConcept WriteCallback = std::function<void(boost::system::error_code, IServiceSession*)>>
class WebsocketSessionsFactory final : public ISessionsFactory
{
private:
    using IAuthorizer = authorizer::IAuthorizer;
    using InternalSessionsManager = base_service_chassis::InternalSessionsManager;
    using tcp = boost::asio::ip::tcp;

public:
    WebsocketSessionsFactory() = delete;

    // clang-format off
    WebsocketSessionsFactory(
            std::shared_ptr<InternalSessionsManager>, 
            std::shared_ptr<IAuthorizer>,
            ReadCallback = [](const std::string&, boost::system::error_code, IServiceSession*) {},
            AcceptCallback = [](boost::system::error_code, IServiceSession*) {},
            WriteCallback = [](boost::system::error_code, IServiceSession*) {}) noexcept;
    // clang-format on

    WebsocketSessionsFactory(const WebsocketSessionsFactory&) noexcept = default;
    WebsocketSessionsFactory(WebsocketSessionsFactory&&) noexcept = default;

    WebsocketSessionsFactory& operator=(const WebsocketSessionsFactory&) noexcept = default;
    WebsocketSessionsFactory& operator=(WebsocketSessionsFactory&&) noexcept = default;

    ~WebsocketSessionsFactory() final = default;

    std::shared_ptr<IServiceSession> GetSession(boost::asio::ip::tcp::socket&&) final;

private:
    ReadCallback m_readCallback;
    AcceptCallback m_acceptCallback;
    WriteCallback m_writeCallback;
};
} // namespace inklink::server_network

namespace inklink::server_network
{
// clang-format off
template <StringErrorCodeSessionCallbackConcept ReadCallback, ErrorCodeAndSessionCallbackConcept AcceptCallback,
          ErrorCodeCallbackConcept WriteCallback> inline
WebsocketSessionsFactory<ReadCallback, AcceptCallback, WriteCallback>::WebsocketSessionsFactory(
        std::shared_ptr<InternalSessionsManager> manager, 
        std::shared_ptr<IAuthorizer> auth, 
        ReadCallback readCallback,
        AcceptCallback acceptCallback, 
        WriteCallback writeCallback) noexcept
        : ISessionsFactory{std::move(manager), std::move(auth)},
          m_readCallback{readCallback},
          m_acceptCallback{acceptCallback}, 
          m_writeCallback{writeCallback}
{
}
// clang-format on

template <StringErrorCodeSessionCallbackConcept ReadCallback, ErrorCodeAndSessionCallbackConcept AcceptCallback,
          ErrorCodeCallbackConcept WriteCallback>
inline std::shared_ptr<IServiceSession>
WebsocketSessionsFactory<ReadCallback, AcceptCallback, WriteCallback>::GetSession(boost::asio::ip::tcp::socket&& socket)
{
    return std::make_shared<WebsocketServiceSession<ReadCallback, AcceptCallback, WriteCallback>>(
            std::move(socket), m_readCallback, m_acceptCallback, m_writeCallback);
}
} // namespace inklink::server_network
