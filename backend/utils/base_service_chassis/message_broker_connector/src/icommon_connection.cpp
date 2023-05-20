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

void ICommonConnection::AddAcceptCallback(std::function<void(ConnectType, error_code, IClientSession*)> callback)
{
    m_acceptCallbacks.push_back(callback);
}
void ICommonConnection::AddReadCallback(std::function<void(const std::string&, error_code, IClientSession*)> callback)
{
    m_readCallbacks.push_back(callback);
}
void ICommonConnection::AddWriteCallback(std::function<void(error_code)> callback)
{
    m_writeCallbacks.push_back(callback);
}

Endpoint ICommonConnection::GetEndpointSelf() const noexcept
{
    return m_endpointSelf;
}

} // namespace inklink::base_service_chassis
