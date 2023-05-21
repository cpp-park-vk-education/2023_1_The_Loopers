#include "websocket_session_factory.h"

#include "websocket_service_session.h"

namespace
{
using WebsocketServiceSession = inklink::server_network::WebsocketServiceSession;
}

namespace inklink::server_network
{
// clang-format off
template <ErrorCodeAndSessionCallbackConcept ReadCallback, ErrorCodeAndSessionCallbackConcept AcceptCallback,
          ErrorCodeCallbackConcept WriteCallback>
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

template <ErrorCodeAndSessionCallbackConcept ReadCallback, ErrorCodeAndSessionCallbackConcept AcceptCallback,
          ErrorCodeCallbackConcept WriteCallback>
std::shared_ptr<IServiceSession>
WebsocketSessionsFactory<ReadCallback, AcceptCallback, WriteCallback>::GetSession(boost::asio::ip::tcp::socket&& socket)
{
    return std::make_shared<WebsocketServiceSession<ReadCallback, AcceptCallback, WriteCallback>>(
            std::move(socket), m_readCallback, m_acceptCallback, m_writeCallback);
}
} // namespace inklink::server_network
