#include "icommon_connection.h"

namespace
{
using IClientSession = inklink::client_connector::IClientSession;
using ConnectType = inklink::client_connector::ConnectType;
using error_code = boost::system::error_code;
} // namespace

namespace inklink::base_service_chassis
{
ICommonConnection::ICommonConnection(std::shared_ptr<ILogger> logger) : m_logger{logger}
{
}

void ICommonConnection::SetAcceptCallback(
        std::function<void(ConnectType, error_code, IClientSession*)> callback) noexcept
{
    m_acceptCallback = callback;
}
void ICommonConnection::SetReadCallback(
        std::function<void(const std::string&, error_code, IClientSession*)> callback) noexcept
{
    m_readCallback = callback;
}
void ICommonConnection::SetWriteCallback(std::function<void(error_code, IClientSession*)> callback) noexcept
{
    m_writeCallback = callback;
}

} // namespace inklink::base_service_chassis
