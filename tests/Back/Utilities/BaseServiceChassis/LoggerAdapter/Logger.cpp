#ifndef _ILOGGER_H_
#define _ILOGGER_H_

#include <string>

class ILogger {
  public:
    virtual ~ILogger() = default;

    virtual void LogDebug(const std::string&) {
    }

    virtual void LogInfo(const std::string&) {
    }

    virtual void LogWarning(const std::string&) {
    }

    virtual void LogCritical(const std::string&) {
    }

    virtual void SetLevel(const std::string&) {
    }

    virtual void SetPattern(const std::string&) {
    }

    virtual void SetFile(const std::string& loggerName, const std::string& path) {
    }
};

#endif  // _IDATA_H_
