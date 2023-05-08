#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ievents_handler.h"

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
    MOCK_METHOD(void, send, (const std::string&), (override));
};

class EventsHandlerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        handler = std::make_unique<IEventsHandler>();

        sessionMock = std::make_shared<IServiceSessionMock>(realEndpoint);

        m_chassiss = std::make_shared<IBaseServiceChassis>();
        m_chassiss->m_internalSessionsManager->AddSession({"d1", "u1"}, sessionMock.get());
        handler->SetServiceChassis(m_chassiss);

        int eventId = 1;
        handler->AddSubscriber(eventId, realEndpoint);
    }

    Endpoint realEndpoint{.address = "127.0.0.1", .port = 8080};
    std::unique_ptr<IEventsHandler> handler;
    std::shared_ptr<IServiceSessionMock> sessionMock;
    std::shared_ptr<IBaseServiceChassis> m_chassiss;
};

TEST_F(EventsHandlerTest, SendEvent)
{
    int eventId = 1;
    std::string eventData = "Event Data";

    EXPECT_CALL(*sessionMock, send(testing::_)).Times(1);

    handler->SendEvent(eventId, eventData);
}

TEST_F(EventsHandlerTest, SendEventWithNoSubscribers)
{
    int eventId = 999;
    std::string eventData = "Event Data";

    EXPECT_CALL(*sessionMock, send(testing::_)).Times(0);

    handler->SendEvent(eventId, eventData);
}

TEST_F(EventsHandlerTest, SendEventAfterRemovalOfSubscription)
{
    int eventId = 1;
    handler->RemoveSubscriber(eventId, realEndpoint);

    std::string eventData = "Event Data";

    EXPECT_CALL(*sessionMock, send(testing::_)).Times(0);

    handler->SendEvent(eventId, eventData);
}
