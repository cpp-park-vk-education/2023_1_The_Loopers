#pragma once

#include <string>

namespace inklink::base_service_chassis
{
class ILogger
{
public:
    virtual ~ILogger() = default;

    virtual void LogDebug(const std::string&) = 0;
    virtual void LogInfo(const std::string&) = 0;
    virtual void LogWarning(const std::string&) = 0;
    virtual void LogCritical(const std::string&) = 0;

    virtual void SetLevel(const std::string&) = 0;
    virtual void SetPattern(const std::string&) = 0;
    virtual void SetFile(const std::string& loggerName, const std::string& path) = 0;
};
} // namespace inklink::base_service_chassis
