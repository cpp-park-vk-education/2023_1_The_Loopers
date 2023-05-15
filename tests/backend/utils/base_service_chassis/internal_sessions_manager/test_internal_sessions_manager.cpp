#include "internal_sessions_manager.h"
#include "iservice_session.h"

#include <gtest/gtest.h>
#include <memory>

using namespace inklink;
using IServiceSession = server_network::IServiceSession;
using InternalSessionsManager = base_service_chassis::InternalSessionsManager;
using DocSessionDescriptor = base_service_chassis::DocSessionDescriptor;

class ServiceSessionFake : public IServiceSession
{
public:
    explicit ServiceSessionFake(Endpoint endpoint) : IServiceSession(nullptr, nullptr), m_endpoint{std::move(endpoint)}
    {
    }

    Endpoint GetClientEndpoint() override
    {
        return m_endpoint;
    }
    void RunAsync() override
    {
    }
    void Send(const std::string&) override
    {
    }

private:
    Endpoint m_endpoint;
};

class SessionsManagerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        sessionsManager = std::make_unique<InternalSessionsManager>();

        std::vector<DocSessionDescriptor> docs{{.documentId = "d1", .login = "u1"},
                                               {.documentId = "d2", .login = "u1"},
                                               {.documentId = "d3", .login = "u1"},
                                               {.documentId = "d1", .login = "u2"}};
        std::vector<Endpoint> endpoints{{.address = "127.0.0.1", .port = 80},
                                        {.address = "127.0.0.1", .port = 90},
                                        {.address = "127.0.0.1", .port = 1000},
                                        {.address = "10.62.0.4", .port = 80}};
        for (int i{0}; i < 4; ++i)
        {
            sessions.emplace_back(std::make_shared<ServiceSessionFake>(endpoints[i]));
            sessionsManager->AddSession(docs[i], sessions[i]);
        }
    }

    std::vector<IServiceSession*> VectorOfWeakPtrToRawPtr(std::vector<std::weak_ptr<IServiceSession>> weakVector)
    {
        std::vector<IServiceSession*> result;
        for (auto ptr : weakVector)
        {
            result.push_back(ptr.lock().get());
        }
        return result;
    }

    std::unique_ptr<InternalSessionsManager> sessionsManager;
    std::vector<std::shared_ptr<IServiceSession>> sessions;
};

namespace add_existing
{
TEST_F(SessionsManagerTest, AddExistingSession)
{
    const DocSessionDescriptor descriptor{.documentId = "different", .login = "u1"};

    EXPECT_THROW({ sessionsManager->AddSession(descriptor, sessions[0]); }, std::runtime_error);
}

TEST_F(SessionsManagerTest, AddSessionWithDuplicatedEndpoint)
{
    Endpoint existingEndpoint{sessions[0]->GetClientEndpoint()};
    const DocSessionDescriptor descriptor{.documentId = "different", .login = "u1"};
    auto session = std::make_shared<ServiceSessionFake>(existingEndpoint);

    EXPECT_THROW(sessionsManager->AddSession(descriptor, session), std::runtime_error);
}

TEST_F(SessionsManagerTest, AddSessionWithDuplicatedDescriptor)
{
    const DocSessionDescriptor descriptor{.documentId = "d1", .login = "u1"};
    IServiceSession* session = new ServiceSessionFake({"new ip", 8080});

    EXPECT_THROW({ sessionsManager->AddSession(descriptor, sessions[0]); }, std::runtime_error);

    delete session;
}
} // namespace add_existing

namespace get_good
{
TEST_F(SessionsManagerTest, GetSessionByEndpoint)
{
    IServiceSession* expected = sessions[0].get();
    IServiceSession* actual = sessionsManager->GetSession({.address = "127.0.0.1", .port = 80}).lock().get();

    EXPECT_EQ(expected, actual);
}

TEST_F(SessionsManagerTest, GetSessionsByDocDescriptor)
{
    const DocSessionDescriptor descriptor{.documentId = "d1", .login = "u1"};

    IServiceSession* expected = sessions[0].get();
    IServiceSession* actual = sessionsManager->GetSession(descriptor).lock().get();

    EXPECT_EQ(expected, actual);
}

TEST_F(SessionsManagerTest, GetSessionsByUser)
{
    const std::vector<IServiceSession*> expected{sessions[3].get()};
    auto actual = VectorOfWeakPtrToRawPtr(sessionsManager->GetSessionsByUser("u2"));

    ASSERT_EQ(actual.size(), 1);
    EXPECT_EQ(expected, actual);
}

TEST_F(SessionsManagerTest, GetMultipleSessionsByUser)
{
    auto actual = VectorOfWeakPtrToRawPtr(sessionsManager->GetSessionsByUser("u1"));

    EXPECT_EQ(actual.size(), 3);
}

TEST_F(SessionsManagerTest, GetSessionsByDoc)
{
    const std::vector<IServiceSession*> expected{sessions[1].get()};
    auto actual = VectorOfWeakPtrToRawPtr(sessionsManager->GetSessionsByDocument("d2"));

    ASSERT_EQ(actual.size(), 1);
    EXPECT_EQ(expected, actual);
}

TEST_F(SessionsManagerTest, GetMultipleSessionsByDoc)
{
    auto actual = VectorOfWeakPtrToRawPtr(sessionsManager->GetSessionsByDocument("d1"));

    EXPECT_EQ(actual.size(), 2);
}

TEST_F(SessionsManagerTest, GetSessionByEndpointAfterRemovalByDescriptor)
{
    const DocSessionDescriptor descriptor{.documentId = "d2", .login = "u1"};
    sessionsManager->RemoveSession(descriptor);

    IServiceSession* expected = sessions[0].get();
    IServiceSession* actual = sessionsManager->GetSession({.address = "127.0.0.1", .port = 80}).lock().get();

    EXPECT_EQ(expected, actual);
}

TEST_F(SessionsManagerTest, GetSessionsByDocDescriptorAfterRemovalByDescriptor)
{
    const DocSessionDescriptor delDescriptor{.documentId = "d2", .login = "u1"};
    sessionsManager->RemoveSession(delDescriptor);

    const DocSessionDescriptor descriptor{.documentId = "d1", .login = "u1"};

    IServiceSession* expected = sessions[0].get();
    IServiceSession* actual = sessionsManager->GetSession(descriptor).lock().get();

    EXPECT_EQ(expected, actual);
}

TEST_F(SessionsManagerTest, GetSessionsByUserAfterRemovalByDescriptor)
{
    const DocSessionDescriptor descriptor{.documentId = "d2", .login = "u1"};
    sessionsManager->RemoveSession(descriptor);

    const std::vector<IServiceSession*> expected{sessions[3].get()};
    auto actual = VectorOfWeakPtrToRawPtr(sessionsManager->GetSessionsByUser("u2"));

    ASSERT_EQ(actual.size(), 1);
    EXPECT_EQ(expected, actual);
}

TEST_F(SessionsManagerTest, GetSessionsByDocAfterRemovalByDescriptor)
{
    const DocSessionDescriptor descriptor{.documentId = "d1", .login = "u1"};
    sessionsManager->RemoveSession(descriptor);

    const std::vector<IServiceSession*> expected{sessions[1].get()};
    auto actual = VectorOfWeakPtrToRawPtr(sessionsManager->GetSessionsByDocument("d2"));

    ASSERT_EQ(actual.size(), 1);
    EXPECT_EQ(expected, actual);
}

TEST_F(SessionsManagerTest, GetSessionByEndpointAfterRemovalBySessionPointer)
{
    sessionsManager->RemoveSession(sessions[1].get());

    IServiceSession* expected = sessions[0].get();
    IServiceSession* actual = sessionsManager->GetSession({.address = "127.0.0.1", .port = 80}).lock().get();

    EXPECT_EQ(expected, actual);
}

TEST_F(SessionsManagerTest, GetSessionsByDocDescriptorAfterRemovalBySessionPointer)
{
    sessionsManager->RemoveSession(sessions[1].get());

    const DocSessionDescriptor descriptor{.documentId = "d1", .login = "u1"};

    IServiceSession* expected = sessions[0].get();
    IServiceSession* actual = sessionsManager->GetSession(descriptor).lock().get();

    EXPECT_EQ(expected, actual);
}

TEST_F(SessionsManagerTest, GetSessionsByUserAfterRemovalBySessionPointer)
{
    sessionsManager->RemoveSession(sessions[1].get());

    const std::vector<IServiceSession*> expected{sessions[3].get()};
    auto actual = VectorOfWeakPtrToRawPtr(sessionsManager->GetSessionsByUser("u2"));

    ASSERT_EQ(actual.size(), 1);
    EXPECT_EQ(expected, actual);
}

TEST_F(SessionsManagerTest, GetSessionsByDocAfterRemovalBySessionPointer)
{
    sessionsManager->RemoveSession(sessions[0].get());

    const std::vector<IServiceSession*> expected{sessions[1].get()};
    auto actual = VectorOfWeakPtrToRawPtr(sessionsManager->GetSessionsByDocument("d2"));

    ASSERT_EQ(actual.size(), 1);
    EXPECT_EQ(expected, actual);
}
} // namespace get_good

namespace test_delete
{
TEST_F(SessionsManagerTest, DeleteNonExistingByPointer)
{
    IServiceSession* session = new ServiceSessionFake({});
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
    const DocSessionDescriptor descriptor{.documentId = "non existing", .login = "u1"};
    EXPECT_NO_THROW(sessionsManager->RemoveSession(descriptor));
}

TEST_F(SessionsManagerTest, DoubleRemovalByDescriptor)
{
    const DocSessionDescriptor descriptor{.documentId = "d1", .login = "u1"};
    sessionsManager->RemoveSession(descriptor);
    EXPECT_NO_THROW(sessionsManager->RemoveSession(descriptor));
}
} // namespace test_delete

namespace no_session
{
TEST_F(SessionsManagerTest, NoSessionByEndpoint)
{
    IServiceSession* expected = nullptr;
    IServiceSession* actual = sessionsManager->GetSession({.address = "non existing", .port = 80}).lock().get();

    EXPECT_EQ(expected, actual);
}

TEST_F(SessionsManagerTest, NoSessionsByDocDescriptor)
{
    const DocSessionDescriptor descriptor{.documentId = "non existing", .login = "u1"};

    IServiceSession* expected = nullptr;
    IServiceSession* actual = sessionsManager->GetSession(descriptor).lock().get();

    EXPECT_EQ(expected, actual);
}

TEST_F(SessionsManagerTest, NoSessionsByUser)
{
    const std::vector<IServiceSession*> expected{};
    auto actual = VectorOfWeakPtrToRawPtr(sessionsManager->GetSessionsByUser("u2"));

    EXPECT_EQ(actual.size(), expected.size());
}

TEST_F(SessionsManagerTest, NoSessionsByDoc)
{
    const std::vector<IServiceSession*> expected{};
    auto actual = VectorOfWeakPtrToRawPtr(sessionsManager->GetSessionsByDocument("d2"));

    EXPECT_EQ(actual.size(), expected.size());
}

TEST_F(SessionsManagerTest, NoSessionByEndpointAfterRemovalByDescriptor)
{
    const DocSessionDescriptor descriptor{.documentId = "d1", .login = "u1"};
    sessionsManager->RemoveSession(descriptor);

    IServiceSession* expected = nullptr;
    IServiceSession* actual = sessionsManager->GetSession({.address = "127.0.0.1", .port = 80}).lock().get();

    EXPECT_EQ(expected, actual);
}

TEST_F(SessionsManagerTest, NoSessionsByDocDescriptorAfterRemovalByDescriptor)
{
    const DocSessionDescriptor descriptor{.documentId = "d1", .login = "u1"};
    sessionsManager->RemoveSession(descriptor);

    IServiceSession* expected = nullptr;
    IServiceSession* actual = sessionsManager->GetSession(descriptor).lock().get();

    EXPECT_EQ(expected, actual);
}

TEST_F(SessionsManagerTest, NoSessionsByUserAfterRemovalByDescriptor)
{
    // Remove a session
    const DocSessionDescriptor descriptor{.documentId = "d1", .login = "u2"};
    sessionsManager->RemoveSession(descriptor);

    const std::vector<IServiceSession*> expected{};
    auto actual = VectorOfWeakPtrToRawPtr(sessionsManager->GetSessionsByUser("u2"));

    EXPECT_EQ(actual.size(), expected.size());
}

TEST_F(SessionsManagerTest, NoSessionsByDocAfterRemovalByDescriptor)
{
    // Remove a session
    const DocSessionDescriptor descriptor{.documentId = "d2", .login = "u1"};
    sessionsManager->RemoveSession(descriptor);

    const std::vector<IServiceSession*> expected{};
    auto actual = VectorOfWeakPtrToRawPtr(sessionsManager->GetSessionsByDocument("d2"));

    EXPECT_EQ(actual.size(), expected.size());
}

TEST_F(SessionsManagerTest, NoSessionByEndpointAfterRemovalBySessionPointer)
{
    sessionsManager->RemoveSession(sessions[0].get());

    IServiceSession* expected = nullptr;
    IServiceSession* actual = sessionsManager->GetSession({.address = "127.0.0.1", .port = 80}).lock().get();

    EXPECT_EQ(expected, actual);
}

TEST_F(SessionsManagerTest, NoSessionsByDocDescriptorAfterRemovalBySessionPointer)
{
    const DocSessionDescriptor descriptor{.documentId = "d1", .login = "u1"};
    sessionsManager->RemoveSession(sessions[0].get());

    IServiceSession* expected = nullptr;
    IServiceSession* actual = sessionsManager->GetSession(descriptor).lock().get();

    EXPECT_EQ(expected, actual);
}

TEST_F(SessionsManagerTest, NoSessionsByUserAfterRemovalBySessionPointer)
{
    sessionsManager->RemoveSession(sessions[3].get());

    const std::vector<IServiceSession*> expected{};
    auto actual = VectorOfWeakPtrToRawPtr(sessionsManager->GetSessionsByUser("u2"));

    EXPECT_EQ(actual.size(), expected.size());
}

TEST_F(SessionsManagerTest, NoSessionsByDocAfterRemovalBySessionPointer)
{
    sessionsManager->RemoveSession(sessions[1].get());

    const std::vector<IServiceSession*> expected{};
    auto actual = VectorOfWeakPtrToRawPtr(sessionsManager->GetSessionsByDocument("d2"));

    EXPECT_EQ(actual.size(), expected.size());
}
} // namespace no_session
