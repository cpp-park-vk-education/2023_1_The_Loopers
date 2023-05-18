#include "idraw_conflict_resolver.h"
#include "inklink_global.h"

#include <chrono>
#include <gtest/gtest.h>
#include <vector>

using namespace inklink;
using namespace inklink::service_simultaneous_access;

using namespace std::chrono_literals;

class TimeBasedDrawConflictResolverTest : public testing::Test
{
public:
    IDrawConflictResolver* m_Resolver;
    Endpoint endpoint1 = {"127.0.0.1", 2154};
    Endpoint endpoint2 = {"45.5.4.1", 2144};

protected:
    void SetUp() override
    {
        m_Resolver = nullptr; // new IDrawConflictResolver();
    }

    void TearDown() override
    {
        delete m_Resolver;
    }
};

TEST_F(TimeBasedDrawConflictResolverTest, EmptyInput)
{
    std::vector<DrawAction> expected;
    auto actual = m_Resolver->Resolve(expected);

    EXPECT_EQ(actual.size(), 0);
}

TEST_F(TimeBasedDrawConflictResolverTest, SingleActionInput)
{
    std::vector<DrawAction> expected{
            {ResolverActionType::kInsertion, "figure1", endpoint1, std::chrono::system_clock::now(), nullptr}};
    auto actual = m_Resolver->Resolve(expected);

    EXPECT_EQ(actual[0], expected[0]);
}

TEST_F(TimeBasedDrawConflictResolverTest, MultipleInsertionsOneClientOneFigure)
{
    std::vector<DrawAction> input = {
            {ResolverActionType::kInsertion, "figure1", endpoint1, std::chrono::system_clock::now() - 10s, nullptr},
            {ResolverActionType::kInsertion, "figure1", endpoint1, std::chrono::system_clock::now(), nullptr},
            {ResolverActionType::kInsertion, "figure1", endpoint1, std::chrono::system_clock::now() - 5s, nullptr}};

    std::vector<DrawAction> expected = input;
    std::sort(expected.begin(), expected.end(),
              [](const DrawAction& a, const DrawAction& b) { return a.time < b.time; });

    auto actual = m_Resolver->Resolve(input);

    ASSERT_EQ(actual.size(), expected.size());
    for (size_t i = 0; i < actual.size(); ++i)
    {
        EXPECT_EQ(actual[i], expected[i]);
    }
}

TEST_F(TimeBasedDrawConflictResolverTest, MultipleActionTypesOneClientOneFigure)
{
    std::vector<DrawAction> input = {
            {ResolverActionType::kInsertion, "figure1", endpoint1, std::chrono::system_clock::now() - 10s, nullptr},
            {ResolverActionType::kFormat, "figure1", endpoint1, std::chrono::system_clock::now(), nullptr},
            {ResolverActionType::kSelect, "figure1", endpoint1, std::chrono::system_clock::now() - 5s, nullptr}};

    std::vector<DrawAction> expected = input;
    std::sort(expected.begin(), expected.end(),
              [](const DrawAction& a, const DrawAction& b) { return a.time < b.time; });

    auto actual = m_Resolver->Resolve(input);

    ASSERT_EQ(actual.size(), expected.size());
    for (size_t i = 0; i < actual.size(); ++i)
    {
        EXPECT_EQ(actual[i], expected[i]);
    }
}

TEST_F(TimeBasedDrawConflictResolverTest, TwoClientsOneFigure)
{
    std::vector<DrawAction> input = {
            {ResolverActionType::kInsertion, "figure1", endpoint1, std::chrono::system_clock::now() - 15s, nullptr},
            {ResolverActionType::kInsertion, "figure1", endpoint2, std::chrono::system_clock::now() - 10s, nullptr},
            {ResolverActionType::kInsertion, "figure1", endpoint1, std::chrono::system_clock::now(), nullptr},
            {ResolverActionType::kInsertion, "figure1", endpoint1, std::chrono::system_clock::now() - 5s, nullptr}};

    std::vector<DrawAction> expected = input;
    std::sort(expected.begin(), expected.end(),
              [](const DrawAction& a, const DrawAction& b) { return a.time < b.time; });

    auto actual = m_Resolver->Resolve(input);

    ASSERT_EQ(actual.size(), expected.size());
    for (size_t i = 0; i < actual.size(); ++i)
    {
        EXPECT_EQ(actual[i], expected[i]);
    }
}

TEST_F(TimeBasedDrawConflictResolverTest, DeletionInMiddle)
{
    std::vector<DrawAction> input = {
            {ResolverActionType::kInsertion, "figure1", endpoint1, std::chrono::system_clock::now() - 10s, nullptr},
            {ResolverActionType::kFormat, "figure1", endpoint1, std::chrono::system_clock::now() - 5s, nullptr},
            {ResolverActionType::kDeletion, "figure1", endpoint2, std::chrono::system_clock::now() - 3s, nullptr},
            {ResolverActionType::kInsertion, "figure1", endpoint1, std::chrono::system_clock::now(), nullptr}};

    std::vector<DrawAction> expected = {input[0], input[1], input[2]};

    std::vector<DrawAction> actual = m_Resolver->Resolve(input);

    ASSERT_EQ(actual.size(), expected.size());
    for (size_t i = 0; i < actual.size(); ++i)
    {
        EXPECT_EQ(actual[i], expected[i]);
    }
}

TEST_F(TimeBasedDrawConflictResolverTest, DeletionAfterOtherSelected)
{
    std::vector<DrawAction> input = {
            {ResolverActionType::kSelect, "figure1", endpoint1, std::chrono::system_clock::now() - 10s, nullptr},
            {ResolverActionType::kDeletion, "figure1", endpoint2, std::chrono::system_clock::now() - 3s, nullptr},
    };

    std::vector<DrawAction> expected = input;

    std::vector<DrawAction> actual = m_Resolver->Resolve(input);

    ASSERT_EQ(actual.size(), expected.size());
    for (size_t i = 0; i < actual.size(); ++i)
    {
        EXPECT_EQ(actual[i], expected[i]);
    }
}

TEST_F(TimeBasedDrawConflictResolverTest, OperationsOnDifferentFigures)
{
    std::vector<DrawAction> input = {
            {ResolverActionType::kInsertion, "figure1", endpoint1, std::chrono::system_clock::now() - 10s, nullptr},
            {ResolverActionType::kDeletion, "figure2", endpoint1, std::chrono::system_clock::now() - 5s, nullptr},
            {ResolverActionType::kFormat, "figure3", endpoint1, std::chrono::system_clock::now() - 9s, nullptr},
            {ResolverActionType::kSelect, "figure4", endpoint1, std::chrono::system_clock::now(), nullptr}};

    // Expected result is the same as the input, as there are no conflicts to Resolve
    std::vector<DrawAction> expected = input;

    // Resolve conflicts
    std::vector<DrawAction> actual = m_Resolver->Resolve(input);

    // Verify the Resolved input match the expected result
    ASSERT_EQ(actual.size(), expected.size());
    for (size_t i = 0; i < actual.size(); ++i)
    {
        EXPECT_EQ(actual[i], expected[i]);
    }
}
