#include "blocking_draw_resolver.h"
#include "inklink_global.h"

#include <chrono>
#include <gtest/gtest.h>
#include <vector>

using namespace inklink;
using namespace inklink::service_simultaneous_access;

using namespace std::chrono_literals;

// Test cases for resolver that allows only one user to modify figure
// Blocking starts when user selects figure and ends when he deselects it
// therefore input must not be ill-formated (selection action should be passed each time if any)

class BlockingDrawConflictResolverTest : public testing::Test
{
public:
    IDrawConflictResolver* m_Resolver;
    Endpoint endpoint1 = {"127.0.0.1", 2154};
    Endpoint endpoint2 = {"45.5.4.1", 2144};

protected:
    void SetUp() override
    {
        m_Resolver = new BlockingDrawResolver();
    }

    void TearDown() override
    {
        delete m_Resolver;
    }
};

TEST_F(BlockingDrawConflictResolverTest, EmptyInput)
{
    std::vector<DrawAction> expected;
    auto actual = m_Resolver->Resolve(expected);

    EXPECT_EQ(actual.size(), 0);
}

TEST_F(BlockingDrawConflictResolverTest, NoSelectionInput)
{
    std::vector<DrawAction> input{
            {ResolverActionType::kInsertion, "figure1", endpoint1, std::chrono::system_clock::now(), nullptr},
            {ResolverActionType::kInsertion, "figure1", endpoint1, std::chrono::system_clock::now() - 5s, nullptr}};

    // result should be empty because there is no selection for endpoint1
    std::vector<DrawAction> expected;
    auto actual = m_Resolver->Resolve(input);

    EXPECT_EQ(actual.size(), expected.size());
}

TEST_F(BlockingDrawConflictResolverTest, OneUserGoodOtherNoSelectionDifferentFigures)
{
    std::vector<DrawAction> input{
            {ResolverActionType::kSelect, "figure1", endpoint1, std::chrono::system_clock::now() - 5s, nullptr},
            {ResolverActionType::kInsertion, "figureOther", endpoint2, std::chrono::system_clock::now() - 3s, nullptr},
            {ResolverActionType::kInsertion, "figure1", endpoint1, std::chrono::system_clock::now(), nullptr}};

    std::vector<DrawAction> expected{
            {ResolverActionType::kSelect, "figure1", endpoint1, std::chrono::system_clock::now() - 5s, nullptr},
            {ResolverActionType::kInsertion, "figure1", endpoint1, std::chrono::system_clock::now(), nullptr}};
    auto actual = m_Resolver->Resolve(input);

    ASSERT_EQ(actual.size(), expected.size());
    for (size_t i = 0; i < actual.size(); ++i)
    {
        EXPECT_EQ(actual[i], expected[i]);
    }
}

TEST_F(BlockingDrawConflictResolverTest, OneUserGoodOtherNoSelectionOneFigure)
{
    std::vector<DrawAction> input{
            {ResolverActionType::kSelect, "figure1", endpoint1, std::chrono::system_clock::now() - 5s, nullptr},
            {ResolverActionType::kInsertion, "figure1", endpoint2, std::chrono::system_clock::now() - 3s, nullptr},
            {ResolverActionType::kInsertion, "figure1", endpoint1, std::chrono::system_clock::now(), nullptr}};

    std::vector<DrawAction> expected{
            {ResolverActionType::kSelect, "figure1", endpoint1, std::chrono::system_clock::now() - 5s, nullptr},
            {ResolverActionType::kInsertion, "figure1", endpoint1, std::chrono::system_clock::now(), nullptr}};
    auto actual = m_Resolver->Resolve(input);

    ASSERT_EQ(actual.size(), expected.size());
    for (size_t i = 0; i < actual.size(); ++i)
    {
        EXPECT_EQ(actual[i], expected[i]);
    }
}

TEST_F(BlockingDrawConflictResolverTest, NoDeselectSoFar)
{
    std::vector<DrawAction> input = {
            {ResolverActionType::kSelect, "figure1", endpoint1, std::chrono::system_clock::now() - 10s, nullptr},
            {ResolverActionType::kInsertion, "figure1", endpoint1, std::chrono::system_clock::now() - 5s, nullptr},
            {ResolverActionType::kFormat, "figure1", endpoint1, std::chrono::system_clock::now() - 3s, nullptr},
            {ResolverActionType::kInsertion, "figure1", endpoint1, std::chrono::system_clock::now(), nullptr}};

    std::vector<DrawAction> expected = input;

    std::vector<DrawAction> actual = m_Resolver->Resolve(input);

    ASSERT_EQ(actual.size(), expected.size());
    for (size_t i = 0; i < actual.size(); ++i)
    {
        EXPECT_EQ(actual[i], expected[i]);
    }
}

TEST_F(BlockingDrawConflictResolverTest, FullCycle)
{
    std::vector<DrawAction> input = {
            {ResolverActionType::kSelect, "figure1", endpoint1, std::chrono::system_clock::now() - 10s, nullptr},
            {ResolverActionType::kInsertion, "figure1", endpoint1, std::chrono::system_clock::now() - 5s, nullptr},
            {ResolverActionType::kFormat, "figure1", endpoint1, std::chrono::system_clock::now() - 3s, nullptr},
            {ResolverActionType::kDeselect, "figure1", endpoint1, std::chrono::system_clock::now(), nullptr}};

    std::vector<DrawAction> expected = input;

    std::vector<DrawAction> actual = m_Resolver->Resolve(input);

    ASSERT_EQ(actual.size(), expected.size());
    for (size_t i = 0; i < actual.size(); ++i)
    {
        EXPECT_EQ(actual[i], expected[i]);
    }
}

TEST_F(BlockingDrawConflictResolverTest, MultipleSelections)
{
    std::vector<DrawAction> input = {
            {ResolverActionType::kSelect, "figure1", endpoint1, std::chrono::system_clock::now() - 10s, nullptr},
            {ResolverActionType::kSelect, "figure1", endpoint1, std::chrono::system_clock::now() - 5s, nullptr},
            {ResolverActionType::kFormat, "figure1", endpoint1, std::chrono::system_clock::now() - 3s, nullptr},
            {ResolverActionType::kDeselect, "figure1", endpoint1, std::chrono::system_clock::now(), nullptr}};

    std::vector<DrawAction> expected = {
            {ResolverActionType::kSelect, "figure1", endpoint1, std::chrono::system_clock::now() - 10s, nullptr},
            {ResolverActionType::kFormat, "figure1", endpoint1, std::chrono::system_clock::now() - 3s, nullptr},
            {ResolverActionType::kDeselect, "figure1", endpoint1, std::chrono::system_clock::now(), nullptr}};

    std::vector<DrawAction> actual = m_Resolver->Resolve(input);

    ASSERT_EQ(actual.size(), expected.size());
    for (size_t i = 0; i < actual.size(); ++i)
    {
        EXPECT_EQ(actual[i], expected[i]);
    }
}

TEST_F(BlockingDrawConflictResolverTest, OtherUserTriesSelectWhenBlocked)
{
    std::vector<DrawAction> input = {
            {ResolverActionType::kSelect, "figure1", endpoint1, std::chrono::system_clock::now() - 10s, nullptr},
            {ResolverActionType::kSelect, "figure1", endpoint2, std::chrono::system_clock::now() - 5s, nullptr},
            {ResolverActionType::kFormat, "figure1", endpoint1, std::chrono::system_clock::now() - 3s, nullptr}};

    std::vector<DrawAction> expected = {
            {ResolverActionType::kSelect, "figure1", endpoint1, std::chrono::system_clock::now() - 10s, nullptr},
            {ResolverActionType::kFormat, "figure1", endpoint1, std::chrono::system_clock::now() - 3s, nullptr}};

    std::vector<DrawAction> actual = m_Resolver->Resolve(input);

    ASSERT_EQ(actual.size(), expected.size());
    for (size_t i = 0; i < actual.size(); ++i)
    {
        EXPECT_EQ(actual[i], expected[i]);
    }
}

TEST_F(BlockingDrawConflictResolverTest, OtherUserSelectsAfterDeselect)
{
    std::vector<DrawAction> input = {
            {ResolverActionType::kSelect, "figure1", endpoint1, std::chrono::system_clock::now() - 10s, nullptr},
            {ResolverActionType::kFormat, "figure1", endpoint1, std::chrono::system_clock::now() - 3s, nullptr},
            {ResolverActionType::kDeselect, "figure1", endpoint1, std::chrono::system_clock::now() - 1s, nullptr},
            {ResolverActionType::kSelect, "figure1", endpoint2, std::chrono::system_clock::now(), nullptr}};

    std::vector<DrawAction> expected = input;

    std::vector<DrawAction> actual = m_Resolver->Resolve(input);

    ASSERT_EQ(actual.size(), expected.size());
    for (size_t i = 0; i < actual.size(); ++i)
    {
        EXPECT_EQ(actual[i], expected[i]);
    }
}

TEST_F(BlockingDrawConflictResolverTest, UsersSelectsDifferentFiguresIsOk)
{
    std::vector<DrawAction> input = {
            {ResolverActionType::kSelect, "figure1", endpoint1, std::chrono::system_clock::now() - 10s, nullptr},
            {ResolverActionType::kSelect, "figure2", endpoint2, std::chrono::system_clock::now() - 5s, nullptr},
            {ResolverActionType::kFormat, "figure1", endpoint1, std::chrono::system_clock::now() - 3s, nullptr},
            {ResolverActionType::kFormat, "figure2", endpoint2, std::chrono::system_clock::now(), nullptr}};

    std::vector<DrawAction> expected = input;

    std::vector<DrawAction> actual = m_Resolver->Resolve(input);

    ASSERT_EQ(actual.size(), expected.size());
    for (size_t i = 0; i < actual.size(); ++i)
    {
        EXPECT_EQ(actual[i], expected[i]);
    }
}
