#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

#include "iinternal_sessions_manager.h"

class IServiceSession
{
  public:
    virtual ~IServiceSession() = default;

    virtual void SetManager(IInternalSessionsManager*)
    {
    }
    virtual void SetAuthorizer(IAuthorizer*)
    {
    }
    virtual void SetNetworkAdapter(INetworkStreamAdapter*)
    {
    }

    virtual void run()
    {
    }
    virtual void run_async()
    {
    }
    virtual std::string sync_read()
    {
    }
    virtual void send(const std::string&)
    {
    }
};

// Define a mock class for the IServiceSession interface
class MockServerSession : public IServiceSession
{
  public:
    MOCK_METHOD(void, send, (const std::string&), (override));
};

// Define a fixture class for the IInternalSessionsManager tests
class IInternalSessionsManagerTest : public ::testing::Test
{
  protected:
    // Create a new instance of the IInternalSessionsManager for each test
    void SetUp() override
    {
        m_manager = std::make_unique<IInternalSessionsManager>();
    }

    std::unique_ptr<IInternalSessionsManager> m_manager;
};

class IInternalSessionsManagerTest : public testing::Test
{
  protected:
    std::unique_ptr<IInternalSessionsManager> m_manager;

    void SetUp() override
    {
        // Create a new instance of the resolver before each test
        resolver_ = create_resolver();

        // Initialize the input vector with some random actions and times
        // input_ = {{ResolverActionType::kInsertion, std::chrono::system_clock::now() - 10s},
        //           {kDeletion, std::chrono::system_clock::now() - 5s},
        //           {kFormat, std::chrono::system_clock::now()},
        //           {kSelect, std::chrono::system_clock::now() + 5s},
        //           {kDeselect, std::chrono::system_clock::now() + 10s}};

        input_ = {{ResolverActionType::kInsertion, std::chrono::system_clock::now() - 10s, data_},
                  {ResolverActionType::kInsertion, std::chrono::system_clock::now(), data_},
                  {ResolverActionType::kInsertion, std::chrono::system_clock::now() - 5s, data_}};

        // Initialize the expected output vector with the input vector sorted by time
        expected_output_ = input_;
        std::sort(expected_output_.begin(), expected_output_.end(),
                  [](const DrawAction& a, const DrawAction& b)
                  {
                      return a.time < b.time;
                  });
    }

    void TearDown() override
    {
        // Destroy the resolver after each test
        delete resolver_;
    }

    // Helper function to create an instance of the resolver
    virtual IDrawConflictResolver* create_resolver() const = 0;
};

// Test case for resolver that sorts the actions by time
class TimeBasedDrawConflictResolverTest : public DrawConflictResolverTest
{
  protected:
    IDrawConflictResolver* create_resolver() const override
    {
        // Create a new instance of the time-based resolver
        return new IDrawConflictResolver();
    }
};

TEST_F(IInternalSessionsManagerTest, AddAndRemoveSession)
{
    IInternalSessionsManager manager;
    MockServerSession session1, session2;
    DocSessionDescriptor descriptor1{"doc1", "user1"};
    DocSessionDescriptor descriptor2{"doc2", "user2"};

    // Add sessions
    manager.AddSession(descriptor1, &session1);
    manager.AddSession(descriptor2, &session2);

    // Check that sessions were added
    auto sessions1 = manager.GetSessions(descriptor1);
    EXPECT_EQ(sessions1.size(), 1);
    EXPECT_EQ(sessions1[0], &session1);
    auto sessions2 = manager.GetSessions(descriptor2);
    EXPECT_EQ(sessions2.size(), 1);
    EXPECT_EQ(sessions2[0], &session2);

    // Remove sessions
    manager.RemoveSession(descriptor1);
    manager.RemoveSession(descriptor2);

    // Check that sessions were removed
    sessions1 = manager.GetSessions(descriptor1);
    EXPECT_EQ(sessions1.size(), 0);
    sessions2 = manager.GetSessions(descriptor2);
    EXPECT_EQ(sessions2.size(), 0);
}

TEST_F(IInternalSessionsManagerTest, GetSessionsByDocument)
{
    IInternalSessionsManager manager;
    MockServerSession session1, session2, session3;
    DocSessionDescriptor descriptor1{"doc1", "user1"};
    DocSessionDescriptor descriptor2{"doc2", "user2"};
    DocSessionDescriptor descriptor3{"doc1", "user3"};

    // Add sessions
    manager.AddSession(descriptor1, &session1);
    manager.AddSession(descriptor2, &session2);
    manager.AddSession(descriptor3, &session3);

    // Get sessions by document
    auto sessions = manager.GetSessionsByDocument("doc1");
    EXPECT_EQ(sessions.size(), 2);
    EXPECT_EQ(sessions[0], &session1);
    EXPECT_EQ(sessions[1], &session3);

    sessions = manager.GetSessionsByDocument("doc2");
    EXPECT_EQ(sessions.size(), 1);
    EXPECT_EQ(sessions[0], &session2);
}

TEST_F(IInternalSessionsManagerTest, GetSessionsByUser)
{
    IInternalSessionsManager manager;
    MockServerSession session1, session2, session3;
    DocSessionDescriptor descriptor1{"doc1", "user1"};
    DocSessionDescriptor descriptor2{"doc2", "user2"};
    DocSessionDescriptor descriptor3{"doc3", "user1"};

    // Add sessions
    manager.AddSession(descriptor1, &session1);
    manager.AddSession(descriptor2, &session2);
    manager.AddSession(descriptor3, &session3);

    // Get sessions by user
    auto sessions = manager.GetSessionsByUser("user1");
    EXPECT_EQ(sessions.size(), 2);
    EXPECT_EQ(sessions[0], &session1);
    EXPECT_EQ(sessions[1], &session3);

    sessions = manager.GetSessionsByUser("user2");
    EXPECT_EQ(sessions.size(), 1);
    EXPECT_EQ(sessions[0], &session2);
}

TEST_F(IInternalSessionsManagerTest, GetSessionsByEndpoint)
{
    IInternalSessionsManager manager;
    MockServerSession session1, session2;
    DocSessionDescriptor descriptor1{"doc1", "user1"};
    DocSessionDescriptor descriptor2{"doc2", "user2"};
    Endpoint endpoint1{"127."};
}

////
///
///

////
///

//
///

TEST_F(IInternalSessionsManagerTest, AddSessionTest)
{
    // Create a mock server session and a doc session descriptor
    auto mockSession = std::make_unique<MockServerSession>();
    IInternalSessionsManager::DocSessionDescriptor descriptor = {"doc1", "user1"};

    // Add the session to the manager
    m_manager->AddSession(descriptor, mockSession.get());

    // Verify that the session was added successfully
    ASSERT_EQ(m_manager->GetSessions(descriptor).size(), 1);
}

TEST_F(IInternalSessionsManagerTest, RemoveSessionTest)
{
    // Create a mock server session and a doc session descriptor
    auto mockSession = std::make_unique<MockServerSession>();
    IInternalSessionsManager::DocSessionDescriptor descriptor = {"doc1", "user1"};

    // Add the session to the manager
    m_manager->AddSession(descriptor, mockSession.get());

    // Remove the session from the manager
    m_manager->RemoveSession(descriptor);

    // Verify that the session was removed successfully
    ASSERT_EQ(m_manager->GetSessions(descriptor).size(), 0);
}

TEST_F(IInternalSessionsManagerTest, GetSessionsTest)
{
    // Create mock server sessions and a doc session descriptor
    auto mockSession1 = std::make_unique<MockServerSession>();
    auto mockSession2 = std::make_unique<MockServerSession>();
    IInternalSessionsManager::DocSessionDescriptor descriptor = {"doc1", "user1"};

    // Add the sessions to the manager
    m_manager->AddSession(descriptor, mockSession1.get());
    m_manager->AddSession(descriptor, mockSession2.get());

    // Verify that the sessions were added successfully
    auto sessions = m_manager->GetSessions(descriptor);
    ASSERT_EQ(sessions.size(), 2);
    ASSERT_EQ(sessions[0], mockSession1.get());
    ASSERT_EQ(sessions[1], mockSession2.get());
}

// Add more tests as needed
