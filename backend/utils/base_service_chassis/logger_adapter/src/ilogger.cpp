#include "ilogger.h"

namespace inklink::base_service_chassis
{
LoggerException::LoggerException(std::string msg) : m_msg{std::move(msg)}
{
}

const char* LoggerException::what() const noexcept
{
    return m_msg.c_str();
}

} // namespace inklink::base_service_chassis
