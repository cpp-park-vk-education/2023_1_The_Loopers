#include "icommon_connection.h"

namespace
{
using IClientSession = inklink::client_connector::IClientSession;
using ConnectType = inklink::client_connector::ConnectType;
using error_code = boost::system::error_code;

using AcceptFunctor = std::function<void(ConnectType, error_code, IClientSession*)>;
using ReadFunctor = std::function<void(const std::string&, error_code, IClientSession*)>;
using WriteFunctor = std::function<void(error_code)>;
} // namespace

namespace inklink::base_service_chassis
{
ICommonConnection::ICommonConnection(std::shared_ptr<ILogger> logger) : m_logger{logger}
{
}

void ICommonConnection::AddAcceptCallback(AcceptFunctor callback)
{
    m_acceptCallbacks.push_back(callback);
}
void ICommonConnection::AddReadCallback(ReadFunctor callback)
{
    m_readCallbacks.push_back(callback);
}
void ICommonConnection::AddWriteCallback(WriteFunctor callback)
{
    m_writeCallbacks.push_back(callback);
}

Endpoint ICommonConnection::GetEndpointSelf() const noexcept
{
    return m_endpointSelf;
}

} // namespace inklink::base_service_chassis
