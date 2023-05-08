#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

#include "ibase_service_chassiss.h"

class ISimpleLogger
{
public:
    virtual ~ISimpleLogger() = default;

    virtual void Log(const std::string& message) = 0;
};

class SimpleLoggerAdapter : public ILogger
{
public:
    SimpleLoggerAdapter(ISimpleLogger* logger) : m_logger{logger}
    {
    }

    void LogDebug(const std::string& str) override
    {
        m_logger->Log(str);
    }

    void LogInfo(const std::string& str) override
    {
        m_logger->Log(str);
    }

    void LogWarning(const std::string& str) override
    {
        m_logger->Log(str);
    }

    void LogCritical(const std::string& str) override
    {
        m_logger->Log(str);
    }

private:
    ISimpleLogger* m_logger;
};

class SimpleLoggerMock : public ISimpleLogger
{
public:
    MOCK_METHOD(void, Log, (const std::string&), (override));
};

class BaseServiceChassissTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        sessionsManager = std::make_unique<IInternalSessionsManager>();

        std::vector<IInternalSessionsManager::DocSessionDescriptor> docs{
                {.documentId = "d1", .login = "u1"},
                {.documentId = "d2", .login = "u1"},
                {.documentId = "d3", .login = "u1"},
                {.documentId = "d1", .login = "u2"}};
        std::vector<Endpoint> endpoints{{.address = "127.0.0.1", .port = 80},
                                        {.address = "127.0.0.1", .port = 90},
                                        {.address = "127.0.0.1", .port = 1000},
                                        {.address = "10.62.0.4", .port = 80}};
        for (int i{0}; i < 4; ++i)
        {
            sessions.emplace_back(std::make_unique<ServiceSession_Mock>(endpoints[i]));
            sessionsManager->AddSession(docs[i], sessions[i].get());
        }
    }

    std::unique_ptr<IInternalSessionsManager> sessionsManager;
    std::vector<std::unique_ptr<IServiceSession>> sessions;
};
