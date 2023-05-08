#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "isignals_handler.h"

class ServiceSession_Fake : public IServiceSession
{
public:
    ServiceSession_Fake(Endpoint endpoint) : m_endpoint{std::move(endpoint)}
    {
    }

    Endpoint GetClientEndpoint() override
    {
        return m_endpoint;
    }

private:
    Endpoint m_endpoint;
};

class IServiceSessionMock : public ServiceSession_Fake
{
public:
    IServiceSessionMock(Endpoint endpoint) : ServiceSession_Fake(std::move(endpoint))
    {
    }

    MOCK_METHOD(void, send, (const std::string&), (override));
};

class SignalsHandlerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        handler = std::make_unique<ISignalsHandler>();

        sessionMock = std::make_shared<IServiceSessionMock>(realEndpoint);

        m_chassiss = std::make_shared<IBaseServiceChassis>();
        m_chassiss->m_internalSessionsManager->AddSession({"d1", "u1"}, sessionMock.get());
        handler->SetServiceChassis(m_chassiss);
    }

    Endpoint realEndpoint{.address = "127.0.0.1", .port = 8080};
    std::unique_ptr<ISignalsHandler> handler;
    std::shared_ptr<IServiceSessionMock> sessionMock;
    std::shared_ptr<IBaseServiceChassis> m_chassiss;
};

TEST_F(SignalsHandlerTest, Send)
{
    std::string msg = "Event Data";

    EXPECT_CALL(*sessionMock, send(testing::_)).Times(1);

    handler->Send(realEndpoint, msg);
}

TEST_F(SignalsHandlerTest, SendToNonExisting)
{
    std::string msg = "Event Data";

    EXPECT_CALL(*sessionMock, send(testing::_)).Times(0);

    handler->Send({.address = "NonExisting", .port = 8080}, msg);
}
