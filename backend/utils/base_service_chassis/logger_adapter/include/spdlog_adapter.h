#pragma once

#include "ilogger.h"

#include <filesystem>
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
    ~SpdlogAdapter() override = default;

    void LogDebug(const std::string&) const noexcept override;
    void LogInfo(const std::string&) const noexcept override;
    void LogWarning(const std::string&) const noexcept override;
    void LogCritical(const std::string&) const noexcept override;

    void SetLevel(LogLevel) override;
    void SetPattern(const std::string&) override;
    void SetFile(const std::string& loggerName, const std::string& path) override;
    void SetFile(const std::string& loggerName, const std::filesystem::path& path) override;

private:
    std::shared_ptr<spdlog::logger> m_logger;
};
} // namespace inklink::base_service_chassis
