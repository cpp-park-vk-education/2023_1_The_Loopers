#include "inklink_global.h"
#include "itext_conflict_resolver.h"

#include <chrono>
#include <gtest/gtest.h>
#include <vector>

using namespace inklink;
using namespace inklink::service_simultaneous_access;

using namespace std::chrono_literals;

class TextConflictResolverTest : public testing::Test
{
public:
    ITextConflictResolver* m_Resolver;
    Endpoint endpoint1 = {"127.0.0.1", 2154};
    Endpoint endpoint2 = {"45.5.4.1", 2144};

protected:
    void SetUp() override
    {
        m_Resolver = create_Resolver();
    }

    void TearDown() override
    {
        delete m_Resolver;
    }

    virtual ITextConflictResolver* create_Resolver() const = 0;
};

namespace time_based_Resolver
{
// Test case for Resolver that sorts the input by time
class TimeBasedTextConflictResolverTest : public TextConflictResolverTest
{
protected:
    ITextConflictResolver* create_Resolver() const override
    {
        // Create a new instance of the time-based Resolver
        // return new ITextConflictResolver();
        return nullptr;
    }
};

TEST_F(TimeBasedTextConflictResolverTest, EmptyInput)
{
    std::vector<TextAction> expected;
    auto actual = m_Resolver->Resolve(expected);

    EXPECT_EQ(actual.size(), 0);
}

TEST_F(TimeBasedTextConflictResolverTest, SingleActionInput)
{
    std::vector<TextAction> expected{
            {ResolverActionType::kInsertion, "figure1", endpoint1, std::chrono::system_clock::now(), 0, 1, nullptr}};
    auto actual = m_Resolver->Resolve(expected);

    EXPECT_EQ(actual[0], expected[0]);
}

TEST_F(TimeBasedTextConflictResolverTest, MultipleInsertionsOneClientOneFigure)
{
    std::vector<TextAction> input = {
            {ResolverActionType::kInsertion, "figure1", endpoint1, std::chrono::system_clock::now() - 10s, 0, 1,
             nullptr},
            {ResolverActionType::kInsertion, "figure1", endpoint1, std::chrono::system_clock::now(), 2, 3, nullptr},
            {ResolverActionType::kInsertion, "figure1", endpoint1, std::chrono::system_clock::now() - 5s, 1, 2,
             nullptr}};

    std::vector<TextAction> expected = input;
    std::sort(expected.begin(), expected.end(),
              [](const TextAction& a, const TextAction& b) { return a.time < b.time; });

    auto actual = m_Resolver->Resolve(input);

    ASSERT_EQ(actual.size(), expected.size());
    for (size_t i = 0; i < actual.size(); ++i)
    {
        EXPECT_EQ(actual[i], expected[i]);
    }
}

TEST_F(TimeBasedTextConflictResolverTest, MultipleActionTypesOneClientOneFigure)
{
    std::vector<TextAction> input = {
            {ResolverActionType::kInsertion, "figure1", endpoint1, std::chrono::system_clock::now() - 10s, 0, 1,
             nullptr},
            {ResolverActionType::kFormat, "figure1", endpoint1, std::chrono::system_clock::now(), 0, 1, nullptr},
            {ResolverActionType::kSelect, "figure1", endpoint1, std::chrono::system_clock::now() - 5s, 0, 1, nullptr}};

    std::vector<TextAction> expected = input;
    std::sort(expected.begin(), expected.end(),
              [](const TextAction& a, const TextAction& b) { return a.time < b.time; });

    auto actual = m_Resolver->Resolve(input);

    ASSERT_EQ(actual.size(), expected.size());
    for (size_t i = 0; i < actual.size(); ++i)
    {
        EXPECT_EQ(actual[i], expected[i]);
    }
}

TEST_F(TimeBasedTextConflictResolverTest, TwoClientsOneFigureInsertionsOnly)
{
    std::vector<TextAction> input = {
            {ResolverActionType::kInsertion, "figure1", endpoint1, std::chrono::system_clock::now() - 15s, 0, 1,
             nullptr},
            {ResolverActionType::kInsertion, "figure1", endpoint2, std::chrono::system_clock::now() - 10s, 0, 1,
             nullptr},
            {ResolverActionType::kInsertion, "figure1", endpoint1, std::chrono::system_clock::now(), 1, 2, nullptr}};

    // insertion shifted to the right by one char for input[1] and [2]
    std::vector<TextAction> expected = input;
    expected[1].posStart = 1;
    expected[1].posEnd = 2;
    expected[2].posStart = 2;
    expected[2].posEnd = 3;

    auto actual = m_Resolver->Resolve(input);

    ASSERT_EQ(actual.size(), expected.size());
    for (size_t i = 0; i < actual.size(); ++i)
    {
        EXPECT_EQ(actual[i], expected[i]);
    }
}

TEST_F(TimeBasedTextConflictResolverTest, TwoClientsOneFigureWithDeletion)
{
    std::vector<TextAction> input = {
            {ResolverActionType::kInsertion, "figure1", endpoint1, std::chrono::system_clock::now() - 15s, 0, 1,
             nullptr},
            {ResolverActionType::kDeletion, "figure1", endpoint2, std::chrono::system_clock::now() - 10s, 0, 1,
             nullptr},
            {ResolverActionType::kInsertion, "figure1", endpoint1, std::chrono::system_clock::now(), 10, 20, nullptr}};

    std::vector<TextAction> expected = input;
    // deletion shifted to the right by one char due to insertion by user1
    expected[1].posStart = 1;
    expected[1].posEnd = 2;
    // insertion shifted to the left by one char due to deletion by user2 (it already takes
    // into account insertion by author user1)
    expected[2].posStart = 9;
    expected[2].posEnd = 19;
    std::vector<TextAction> actual = m_Resolver->Resolve(input);

    ASSERT_EQ(actual.size(), expected.size());
    for (size_t i = 0; i < actual.size(); ++i)
    {
        EXPECT_EQ(actual[i], expected[i]);
    }
}

TEST_F(TimeBasedTextConflictResolverTest, OperationsOnDifferentFigures)
{
    std::vector<TextAction> input = {
            {ResolverActionType::kInsertion, "figure1", endpoint1, std::chrono::system_clock::now() - 10s, 0, 1,
             nullptr},
            {ResolverActionType::kDeletion, "figure2", endpoint1, std::chrono::system_clock::now() - 10s, 0, 1,
             nullptr},
            {ResolverActionType::kFormat, "figure1", endpoint1, std::chrono::system_clock::now(), 0, 1, nullptr},
            {ResolverActionType::kSelect, "figure2", endpoint1, std::chrono::system_clock::now(), 0, 1, nullptr}};

    std::vector<TextAction> expected = input;

    std::vector<TextAction> actual = m_Resolver->Resolve(input);

    ASSERT_EQ(actual.size(), expected.size());
    for (size_t i = 0; i < actual.size(); ++i)
    {
        EXPECT_EQ(actual[i], expected[i]);
    }
}

} // namespace time_based_Resolver

namespace blocking_Resolver
{
// it is solved on the level of figures (draw conflict Resolver) or sessions (in case of separate
// text editor)
} // namespace blocking_Resolver
