#include <gtest/gtest.h>

#include <memory>

#include "iinternal_sessions_manager.h"

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

class SessionsManagerTest : public ::testing::Test
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
            sessions.emplace_back(std::make_unique<ServiceSession_Fake>(endpoints[i]));
            sessionsManager->AddSession(docs[i], sessions[i].get());
        }
    }

    std::unique_ptr<IInternalSessionsManager> sessionsManager;
    std::vector<std::unique_ptr<IServiceSession>> sessions;
};

namespace add_existing
{
TEST_F(SessionsManagerTest, AddExistingSession)
{
    IInternalSessionsManager::DocSessionDescriptor descriptor{.documentId = "different",
                                                              .login = "u1"};

    EXPECT_THROW({ sessionsManager->AddSession(descriptor, sessions[0].get()); },
                 std::runtime_error);
}

TEST_F(SessionsManagerTest, AddSessionWithDuplicatedEndpoint)
{
    Endpoint existingEndpoint{sessions[0]->GetClientEndpoint()};
    IInternalSessionsManager::DocSessionDescriptor descriptor{.documentId = "different",
                                                              .login = "u1"};
    IServiceSession* session = new ServiceSession_Fake(existingEndpoint);

    EXPECT_THROW(sessionsManager->AddSession(descriptor, session), std::runtime_error);

    delete session;
}

TEST_F(SessionsManagerTest, AddSessionWithDuplicatedDescriptor)
{
    IInternalSessionsManager::DocSessionDescriptor descriptor{.documentId = "d1", .login = "u1"};
    IServiceSession* session = new ServiceSession_Fake({"new ip", 8080});

    EXPECT_THROW({ sessionsManager->AddSession(descriptor, sessions[0].get()); },
                 std::runtime_error);

    delete session;
}
}  // namespace add_existing

namespace get_good
{
TEST_F(SessionsManagerTest, GetSessionByEndpoint)
{
    IServiceSession* expected = sessions[0].get();
    IServiceSession* actual = sessionsManager->GetSession({.address = "127.0.0.1", .port = 80});

    EXPECT_EQ(expected, actual);
}

TEST_F(SessionsManagerTest, GetSessionsByDocDescriptor)
{
    IInternalSessionsManager::DocSessionDescriptor descriptor{.documentId = "d1", .login = "u1"};

    IServiceSession* expected = sessions[0].get();
    IServiceSession* actual = sessionsManager->GetSession(descriptor);

    EXPECT_EQ(expected, actual);
}

TEST_F(SessionsManagerTest, GetSessionsByUser)
{
    std::vector<IServiceSession*> expected{sessions[3].get()};
    std::vector<IServiceSession*> actual = sessionsManager->GetSessionsByUser("u2");

    ASSERT_EQ(actual.size(), 1);
    EXPECT_EQ(expected, actual);
}

TEST_F(SessionsManagerTest, GetMultipleSessionsByUser)
{
    std::vector<IServiceSession*> actual = sessionsManager->GetSessionsByUser("u1");

    EXPECT_EQ(actual.size(), 3);
}

TEST_F(SessionsManagerTest, GetSessionsByDoc)
{
    std::vector<IServiceSession*> expected{sessions[1].get()};
    std::vector<IServiceSession*> actual = sessionsManager->GetSessionsByDocument("d2");

    ASSERT_EQ(actual.size(), 1);
    EXPECT_EQ(expected, actual);
}

TEST_F(SessionsManagerTest, GetMultipleSessionsByDoc)
{
    std::vector<IServiceSession*> actual = sessionsManager->GetSessionsByDocument("d1");

    EXPECT_EQ(actual.size(), 2);
}

TEST_F(SessionsManagerTest, GetSessionByEndpointAfterRemovalByDescriptor)
{
    IInternalSessionsManager::DocSessionDescriptor descriptor{.documentId = "d2", .login = "u1"};
    sessionsManager->RemoveSession(descriptor);

    IServiceSession* expected = sessions[0].get();
    IServiceSession* actual = sessionsManager->GetSession({.address = "127.0.0.1", .port = 80});

    EXPECT_EQ(expected, actual);
}

TEST_F(SessionsManagerTest, GetSessionsByDocDescriptorAfterRemovalByDescriptor)
{
    IInternalSessionsManager::DocSessionDescriptor delDescriptor{.documentId = "d2", .login = "u1"};
    sessionsManager->RemoveSession(delDescriptor);

    IInternalSessionsManager::DocSessionDescriptor descriptor{.documentId = "d1", .login = "u1"};

    IServiceSession* expected = sessions[0].get();
    IServiceSession* actual = sessionsManager->GetSession(descriptor);

    EXPECT_EQ(expected, actual);
}

TEST_F(SessionsManagerTest, GetSessionsByUserAfterRemovalByDescriptor)
{
    IInternalSessionsManager::DocSessionDescriptor descriptor{.documentId = "d2", .login = "u1"};
    sessionsManager->RemoveSession(descriptor);

    std::vector<IServiceSession*> expected{sessions[3].get()};
    std::vector<IServiceSession*> actual = sessionsManager->GetSessionsByUser("u2");

    ASSERT_EQ(actual.size(), 1);
    EXPECT_EQ(expected, actual);
}

TEST_F(SessionsManagerTest, GetSessionsByDocAfterRemovalByDescriptor)
{
    IInternalSessionsManager::DocSessionDescriptor descriptor{.documentId = "d1", .login = "u1"};
    sessionsManager->RemoveSession(descriptor);

    std::vector<IServiceSession*> expected{sessions[1].get()};
    std::vector<IServiceSession*> actual = sessionsManager->GetSessionsByDocument("d2");

    ASSERT_EQ(actual.size(), 1);
    EXPECT_EQ(expected, actual);
}

TEST_F(SessionsManagerTest, GetSessionByEndpointAfterRemovalBySessionPointer)
{
    sessionsManager->RemoveSession(sessions[1].get());

    IServiceSession* expected = sessions[0].get();
    IServiceSession* actual = sessionsManager->GetSession({.address = "127.0.0.1", .port = 80});

    EXPECT_EQ(expected, actual);
}

TEST_F(SessionsManagerTest, GetSessionsByDocDescriptorAfterRemovalBySessionPointer)
{
    sessionsManager->RemoveSession(sessions[1].get());

    IInternalSessionsManager::DocSessionDescriptor descriptor{.documentId = "d1", .login = "u1"};

    IServiceSession* expected = sessions[0].get();
    IServiceSession* actual = sessionsManager->GetSession(descriptor);

    EXPECT_EQ(expected, actual);
}

TEST_F(SessionsManagerTest, GetSessionsByUserAfterRemovalBySessionPointer)
{
    sessionsManager->RemoveSession(sessions[1].get());

    std::vector<IServiceSession*> expected{sessions[3].get()};
    std::vector<IServiceSession*> actual = sessionsManager->GetSessionsByUser("u2");

    ASSERT_EQ(actual.size(), 1);
    EXPECT_EQ(expected, actual);
}

TEST_F(SessionsManagerTest, GetSessionsByDocAfterRemovalBySessionPointer)
{
    sessionsManager->RemoveSession(sessions[0].get());

    std::vector<IServiceSession*> expected{sessions[1].get()};
    std::vector<IServiceSession*> actual = sessionsManager->GetSessionsByDocument("d2");

    ASSERT_EQ(actual.size(), 1);
    EXPECT_EQ(expected, actual);
}
}  // namespace get_good

namespace test_delete
{
TEST_F(SessionsManagerTest, DeleteNonExistingByPointer)
{
    IServiceSession* session = new IServiceSession();
    EXPECT_NO_THROW(sessionsManager->RemoveSession(session));
    delete session;
}

TEST_F(SessionsManagerTest, DeleteNullptr)
{
    EXPECT_NO_THROW(sessionsManager->RemoveSession(nullptr));
}

TEST_F(SessionsManagerTest, DoubleRemovalByPointer)
{
    sessionsManager->RemoveSession(sessions[0].get());
    EXPECT_NO_THROW(sessionsManager->RemoveSession(sessions[0].get()));
}

TEST_F(SessionsManagerTest, DeleteNonExistingByDescriptor)
{
    IInternalSessionsManager::DocSessionDescriptor descriptor{.documentId = "non existing",
                                                              .login = "u1"};
    EXPECT_NO_THROW(sessionsManager->RemoveSession(descriptor));
}

TEST_F(SessionsManagerTest, DoubleRemovalByDescriptor)
{
    IInternalSessionsManager::DocSessionDescriptor descriptor{.documentId = "d1", .login = "u1"};
    sessionsManager->RemoveSession(descriptor);
    EXPECT_NO_THROW(sessionsManager->RemoveSession(descriptor));
}
}  // namespace test_delete

namespace no_session
{
TEST_F(SessionsManagerTest, NoSessionByEndpoint)
{
    IServiceSession* expected = nullptr;
    IServiceSession* actual = sessionsManager->GetSession({.address = "non existing", .port = 80});

    EXPECT_EQ(expected, actual);
}

TEST_F(SessionsManagerTest, NoSessionsByDocDescriptor)
{
    IInternalSessionsManager::DocSessionDescriptor descriptor{.documentId = "non existing",
                                                              .login = "u1"};

    IServiceSession* expected = nullptr;
    IServiceSession* actual = sessionsManager->GetSession(descriptor);

    EXPECT_EQ(expected, actual);
}

TEST_F(SessionsManagerTest, NoSessionsByUser)
{
    std::vector<IServiceSession*> expected{};
    std::vector<IServiceSession*> actual = sessionsManager->GetSessionsByUser("u2");

    EXPECT_EQ(actual.size(), expected.size());
}

TEST_F(SessionsManagerTest, NoSessionsByDoc)
{
    std::vector<IServiceSession*> expected{};
    std::vector<IServiceSession*> actual = sessionsManager->GetSessionsByDocument("d2");

    EXPECT_EQ(actual.size(), expected.size());
}

TEST_F(SessionsManagerTest, NoSessionByEndpointAfterRemovalByDescriptor)
{
    IInternalSessionsManager::DocSessionDescriptor descriptor{.documentId = "d1", .login = "u1"};
    sessionsManager->RemoveSession(descriptor);

    IServiceSession* expected = nullptr;
    IServiceSession* actual = sessionsManager->GetSession({.address = "127.0.0.1", .port = 80});

    EXPECT_EQ(expected, actual);
}

TEST_F(SessionsManagerTest, NoSessionsByDocDescriptorAfterRemovalByDescriptor)
{
    IInternalSessionsManager::DocSessionDescriptor descriptor{.documentId = "d1", .login = "u1"};
    sessionsManager->RemoveSession(descriptor);

    IServiceSession* expected = nullptr;
    IServiceSession* actual = sessionsManager->GetSession(descriptor);

    EXPECT_EQ(expected, actual);
}

TEST_F(SessionsManagerTest, NoSessionsByUserAfterRemovalByDescriptor)
{
    // Remove a session
    IInternalSessionsManager::DocSessionDescriptor descriptor{.documentId = "d1", .login = "u2"};
    sessionsManager->RemoveSession(descriptor);

    std::vector<IServiceSession*> expected{};
    std::vector<IServiceSession*> actual = sessionsManager->GetSessionsByUser("u2");

    EXPECT_EQ(actual.size(), expected.size());
}

TEST_F(SessionsManagerTest, NoSessionsByDocAfterRemovalByDescriptor)
{
    // Remove a session
    IInternalSessionsManager::DocSessionDescriptor descriptor{.documentId = "d2", .login = "u1"};
    sessionsManager->RemoveSession(descriptor);

    std::vector<IServiceSession*> expected{};
    std::vector<IServiceSession*> actual = sessionsManager->GetSessionsByDocument("d2");

    EXPECT_EQ(actual.size(), expected.size());
}

TEST_F(SessionsManagerTest, NoSessionByEndpointAfterRemovalBySessionPointer)
{
    sessionsManager->RemoveSession(sessions[0].get());

    IServiceSession* expected = nullptr;
    IServiceSession* actual = sessionsManager->GetSession({.address = "127.0.0.1", .port = 80});

    EXPECT_EQ(expected, actual);
}

TEST_F(SessionsManagerTest, NoSessionsByDocDescriptorAfterRemovalBySessionPointer)
{
    IInternalSessionsManager::DocSessionDescriptor descriptor{.documentId = "d1", .login = "u1"};
    sessionsManager->RemoveSession(sessions[0].get());

    IServiceSession* expected = nullptr;
    IServiceSession* actual = sessionsManager->GetSession(descriptor);

    EXPECT_EQ(expected, actual);
}

TEST_F(SessionsManagerTest, NoSessionsByUserAfterRemovalBySessionPointer)
{
    sessionsManager->RemoveSession(sessions[3].get());

    std::vector<IServiceSession*> expected{};
    std::vector<IServiceSession*> actual = sessionsManager->GetSessionsByUser("u2");

    EXPECT_EQ(actual.size(), expected.size());
}

TEST_F(SessionsManagerTest, NoSessionsByDocAfterRemovalBySessionPointer)
{
    sessionsManager->RemoveSession(sessions[1].get());

    std::vector<IServiceSession*> expected{};
    std::vector<IServiceSession*> actual = sessionsManager->GetSessionsByDocument("d2");

    EXPECT_EQ(actual.size(), expected.size());
}
}  // namespace no_session
