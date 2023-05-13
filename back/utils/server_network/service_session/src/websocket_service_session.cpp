#include "websocket_service_session.h"

namespace inklink_service_session
{
template <Do_ErrorCodeAndSession_Concept DoOnRead, Do_ErrorCodeAndSession_Concept DoOnAccept,
          Do_ErrorCode_Concept DoOnWrite>
WebsocketServiceSession<DoOnRead, DoOnAccept, DoOnWrite>::WebsocketServiceSession(
        net::ip::tcp::socket&& socket, DoOnRead doOnRead, DoOnAccept doOnAccept,
        DoOnWrite doOnWrite)
        : IServiceSession(), m_ws(std::move(socket)), m_doOnRead{doOnRead},
          m_doOnAccept{doOnAccept}, m_doOnWrite{doOnWrite}
{
}

template <Do_ErrorCodeAndSession_Concept DoOnRead, Do_ErrorCodeAndSession_Concept DoOnAccept,
          Do_ErrorCode_Concept DoOnWrite>
WebsocketServiceSession<DoOnRead, DoOnAccept, DoOnWrite>::

}  // namespace inklink_service_session
