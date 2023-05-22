#pragma once

#include <exception>
#include <filesystem>
#include <string>

namespace inklink::base_service_chassis
{
enum class LogLevel
{
    kTrace,
    kDebug,
    kInfo,
    kWarn,
    kErr,
    kCritical,
    kOff
};

class LoggerException : public std::exception
{
public:
    explicit LoggerException(std::string msg);
    [[nodiscard]] const char* what() const noexcept override;

private:
    std::string m_msg;
};

class ILogger
{
public:
    virtual ~ILogger() = default;

    virtual void LogDebug(const std::string&) const noexcept = 0;
    virtual void LogInfo(const std::string&) const noexcept = 0;
    virtual void LogWarning(const std::string&) const noexcept = 0;
    virtual void LogCritical(const std::string&) const noexcept = 0;

    virtual void SetLevel(LogLevel) = 0;
    virtual void SetPattern(const std::string&) = 0;
    virtual void SetFile(const std::string& loggerName, const std::string& path) = 0;
    virtual void SetFile(const std::string& loggerName, const std::filesystem::path& path) = 0;
};
} // namespace inklink::base_service_chassis
