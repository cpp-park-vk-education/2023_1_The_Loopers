#include "websocket_session_factory.h"

#include "websocket_service_session.h"

namespace
{
using WebsocketServiceSession = inklink_service_session::WebsocketServiceSession;
}
namespace inklink_sessions_factory
{
template <Do_ErrorCodeAndSession_Concept DoOnRead, Do_ErrorCodeAndSession_Concept DoOnAccept,
          Do_ErrorCode_Concept DoOnWrite>
WebsocketSessionsFactory<DoOnRead, DoOnAccept, DoOnWrite>::WebsocketSessionsFactory(
        DoOnRead doOnRead, DoOnAccept doOnAccept, DoOnWrite doOnWrite) noexcept
        : m_doOnRead{doOnRead}, m_doOnAccept{doOnAccept}, m_doOnWrite{doOnWrite}
{
}

template <Do_ErrorCodeAndSession_Concept DoOnRead, Do_ErrorCodeAndSession_Concept DoOnAccept,
          Do_ErrorCode_Concept DoOnWrite>
std::shared_ptr<IServiceSession>
WebsocketSessionsFactory<DoOnRead, DoOnAccept, DoOnWrite>::GetSession(
        boost::asio::ip::tcp::socket&& socket)
{
    return std::make_shared<WebsocketServiceSession<DoOnRead, DoOnAccept, DoOnWrite>>(
            std::move(socket), m_doOnRead, m_doOnAccept, m_doOnWrite);
}
}  // namespace inklink_sessions_factory
