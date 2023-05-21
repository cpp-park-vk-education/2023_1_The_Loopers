#pragma once

#include "ilogger.h"

#include <memory>
#include <string>

namespace spdlog
{
class logger;
}

namespace inklink::base_service_chassis
{
class SpdlogAdapter final : public ILogger
{
public:
    ~SpdlogAdapter() final = default;

    void LogDebug(const std::string&) const noexcept final;
    void LogInfo(const std::string&) const noexcept final;
    void LogWarning(const std::string&) const noexcept final;
    void LogCritical(const std::string&) const noexcept final;

    void SetLevel(LogLevel) final;
    void SetPattern(const std::string&) final;
    void SetFile(const std::string& loggerName, const std::string& path) final;

private:
    std::shared_ptr<spdlog::logger> m_logger;
};
} // namespace inklink::base_service_chassis
