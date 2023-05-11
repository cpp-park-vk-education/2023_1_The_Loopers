#include <gtest/gtest.h>

#include <chrono>
#include <vector>

#include "global.h"
#include "itext_conflict_resolver.h"

using namespace std::chrono_literals;

class TextConflictResolverTest : public testing::Test
{
public:
    ITextConflictResolver* m_resolver;
    Endpoint endpoint1 = {"127.0.0.1", 2154};
    Endpoint endpoint2 = {"45.5.4.1", 2144};

protected:
    void SetUp() override
    {
        m_resolver = create_resolver();
    }

    void TearDown() override
    {
        delete m_resolver;
    }

    virtual ITextConflictResolver* create_resolver() const = 0;
};

namespace time_based_resolver
{
// Test case for resolver that sorts the input by time
class TimeBasedTextConflictResolverTest : public TextConflictResolverTest
{
protected:
    ITextConflictResolver* create_resolver() const override
    {
        // Create a new instance of the time-based resolver
        return new ITextConflictResolver();
    }
};

TEST_F(TimeBasedTextConflictResolverTest, EmptyInput)
{
    std::vector<TextAction> expected;
    auto actual = m_resolver->resolve(expected);

    EXPECT_EQ(actual.size(), 0);
}

TEST_F(TimeBasedTextConflictResolverTest, SingleActionInput)
{
    std::vector<TextAction> expected{{ResolverActionType::kInsertion, "figure1", endpoint1,
                                      std::chrono::system_clock::now(), 0, 1, nullptr}};
    auto actual = m_resolver->resolve(expected);

    EXPECT_EQ(actual[0], expected[0]);
}

TEST_F(TimeBasedTextConflictResolverTest, MultipleInsertionsOneClientOneFigure)
{
    std::vector<TextAction> input = {{ResolverActionType::kInsertion, "figure1", endpoint1,
                                      std::chrono::system_clock::now() - 10s, 0, 1, nullptr},
                                     {ResolverActionType::kInsertion, "figure1", endpoint1,
                                      std::chrono::system_clock::now(), 2, 3, nullptr},
                                     {ResolverActionType::kInsertion, "figure1", endpoint1,
                                      std::chrono::system_clock::now() - 5s, 1, 2, nullptr}};

    std::vector<TextAction> expected = input;
    std::sort(expected.begin(), expected.end(),
              [](const TextAction& a, const TextAction& b) { return a.time < b.time; });

    auto actual = m_resolver->resolve(input);

    ASSERT_EQ(actual.size(), expected.size());
    for (size_t i = 0; i < actual.size(); ++i)
    {
        EXPECT_EQ(actual[i], expected[i]);
    }
}

TEST_F(TimeBasedTextConflictResolverTest, MultipleActionTypesOneClientOneFigure)
{
    std::vector<TextAction> input = {{ResolverActionType::kInsertion, "figure1", endpoint1,
                                      std::chrono::system_clock::now() - 10s, 0, 1, nullptr},
                                     {ResolverActionType::kFormat, "figure1", endpoint1,
                                      std::chrono::system_clock::now(), 0, 1, nullptr},
                                     {ResolverActionType::kSelect, "figure1", endpoint1,
                                      std::chrono::system_clock::now() - 5s, 0, 1, nullptr}};

    std::vector<TextAction> expected = input;
    std::sort(expected.begin(), expected.end(),
              [](const TextAction& a, const TextAction& b) { return a.time < b.time; });

    auto actual = m_resolver->resolve(input);

    ASSERT_EQ(actual.size(), expected.size());
    for (size_t i = 0; i < actual.size(); ++i)
    {
        EXPECT_EQ(actual[i], expected[i]);
    }
}

TEST_F(TimeBasedTextConflictResolverTest, TwoClientsOneFigureInsertionsOnly)
{
    std::vector<TextAction> input = {{ResolverActionType::kInsertion, "figure1", endpoint1,
                                      std::chrono::system_clock::now() - 15s, 0, 1, nullptr},
                                     {ResolverActionType::kInsertion, "figure1", endpoint2,
                                      std::chrono::system_clock::now() - 10s, 0, 1, nullptr},
                                     {ResolverActionType::kInsertion, "figure1", endpoint1,
                                      std::chrono::system_clock::now(), 1, 2, nullptr}};

    // each action of different user should be transformed
    std::vector<TextAction> expected = {
            {ResolverActionType::kInsertion, "figure1", endpoint1,
             std::chrono::system_clock::now() - 15s, 0, 1, nullptr},
            // second insertion shifted to the right on one char due to insertion by user1
            {ResolverActionType::kInsertion, "figure1", endpoint2,
             std::chrono::system_clock::now() - 10s, 1, 2, nullptr},
            // third insertion shifted to the right on one char due to insertion by user2 (it
            // already takes into account insertion by author user1)
            {ResolverActionType::kInsertion, "figure1", endpoint1, std::chrono::system_clock::now(),
             2, 3, nullptr}};

    auto actual = m_resolver->resolve(input);

    ASSERT_EQ(actual.size(), expected.size());
    for (size_t i = 0; i < actual.size(); ++i)
    {
        EXPECT_EQ(actual[i], expected[i]);
    }
}

TEST_F(TimeBasedTextConflictResolverTest, TwoClientsOneFigureWithDeletion)
{
    std::vector<TextAction> input = {{ResolverActionType::kInsertion, "figure1", endpoint1,
                                      std::chrono::system_clock::now() - 15s, 0, 1, nullptr},
                                     {ResolverActionType::kDeletion, "figure1", endpoint2,
                                      std::chrono::system_clock::now() - 10s, 0, 1, nullptr},
                                     {ResolverActionType::kInsertion, "figure1", endpoint1,
                                      std::chrono::system_clock::now(), 10, 20, nullptr}};

    // each action of different user should be transformed
    std::vector<TextAction> expected = {
            {ResolverActionType::kInsertion, "figure1", endpoint1,
             std::chrono::system_clock::now() - 15s, 0, 1, nullptr},
            // deletion shifted to the right on one char due to insertion by user1
            {ResolverActionType::kDeletion, "figure1", endpoint2,
             std::chrono::system_clock::now() - 10s, 1, 2, nullptr},
            // insertion shifted to the left on one char due to deletion by user2 (it already takes
            // into account insertion by author user1)
            {ResolverActionType::kInsertion, "figure1", endpoint1, std::chrono::system_clock::now(),
             9, 19, nullptr}};
    std::vector<TextAction> actual = m_resolver->resolve(input);

    ASSERT_EQ(actual.size(), expected.size());
    for (size_t i = 0; i < actual.size(); ++i)
    {
        EXPECT_EQ(actual[i], expected[i]);
    }
}

TEST_F(TimeBasedTextConflictResolverTest, OperationsOnDifferentFigures)
{
    std::vector<TextAction> input = {{ResolverActionType::kInsertion, "figure1", endpoint1,
                                      std::chrono::system_clock::now() - 10s, 0, 1, nullptr},
                                     {ResolverActionType::kDeletion, "figure2", endpoint1,
                                      std::chrono::system_clock::now() - 10s, 0, 1, nullptr},
                                     {ResolverActionType::kFormat, "figure1", endpoint1,
                                      std::chrono::system_clock::now(), 0, 1, nullptr},
                                     {ResolverActionType::kSelect, "figure2", endpoint1,
                                      std::chrono::system_clock::now(), 0, 1, nullptr}};

    std::vector<TextAction> expected = input;

    std::vector<TextAction> actual = m_resolver->resolve(input);

    ASSERT_EQ(actual.size(), expected.size());
    for (size_t i = 0; i < actual.size(); ++i)
    {
        EXPECT_EQ(actual[i], expected[i]);
    }
}

}  // namespace time_based_resolver

namespace blocking_resolver
{
// it is solved on the level of figures (draw conflict resolver) or sessions (in case of separate
// text editor)
}  // namespace blocking_resolver
