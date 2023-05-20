#include "spdlog_adapter.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include <exception>
#include <iostream>

namespace inklink::base_service_chassis
{
void SpdlogAdapter::LogDebug(const std::string& message) const noexcept
{
    if (m_logger)
    {
        m_logger->debug(message);
    }
    else
    {
        spdlog::debug(message);
    }
}

void SpdlogAdapter::LogInfo(const std::string& message) const noexcept
{
    if (m_logger)
    {
        m_logger->info(message);
    }
    else
    {
        spdlog::info(message);
    }
}

void SpdlogAdapter::LogWarning(const std::string& message) const noexcept
{
    if (m_logger)
    {
        m_logger->warn(message);
    }
    else
    {
        spdlog::warn(message);
    }
}

void SpdlogAdapter::LogCritical(const std::string& message) const noexcept
{
    if (m_logger)
    {
        m_logger->critical(message);
    }
    else
    {
        spdlog::critical(message);
    }
}

void SpdlogAdapter::SetLevel(LogLevel level)
{
    switch (level)
    {
    case LogLevel::kTrace:
        if (m_logger)
        {
            m_logger->set_level(spdlog::level::trace);
        }
        else
        {
            spdlog::set_level(spdlog::level::trace);
        }
        break;
    case LogLevel::kDebug:
        if (m_logger)
        {
            m_logger->set_level(spdlog::level::debug);
        }
        else
        {
            spdlog::set_level(spdlog::level::debug);
        }
        break;
    case LogLevel::kInfo:
        if (m_logger)
        {
            m_logger->set_level(spdlog::level::info);
        }
        else
        {
            spdlog::set_level(spdlog::level::info);
        }
        break;
    case LogLevel::kWarn:
        if (m_logger)
        {
            m_logger->set_level(spdlog::level::warn);
        }
        else
        {
            spdlog::set_level(spdlog::level::warn);
        }
        break;
    case LogLevel::kErr:
        if (m_logger)
        {
            m_logger->set_level(spdlog::level::err);
        }
        else
        {
            spdlog::set_level(spdlog::level::err);
        }
        break;
    case LogLevel::kCritical:
        if (m_logger)
        {
            m_logger->set_level(spdlog::level::critical);
        }
        else
        {
            spdlog::set_level(spdlog::level::critical);
        }
        break;
    case LogLevel::kOff:
        if (m_logger)
        {
            m_logger->set_level(spdlog::level::off);
        }
        else
        {
            spdlog::set_level(spdlog::level::off);
        }
        break;
    }
}

void SpdlogAdapter::SetPattern(const std::string& pattern)
{
    if (m_logger)
    {
        m_logger->set_pattern(pattern);
    }
    else
    {
        spdlog::set_pattern(pattern);
    }
}

void SpdlogAdapter::SetFile(const std::string& loggerName, const std::string& path)
{
    try
    {
        auto m_logger = spdlog::basic_logger_mt(loggerName, path);
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        throw LoggerException(ex.what());
    }
}
} // namespace inklink::base_service_chassis
