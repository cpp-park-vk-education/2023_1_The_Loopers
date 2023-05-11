#include <chrono>
#include <vector>

#include "global.h"
#include "gtest/gtest.h"
#include "idraw_conflict_resolver.h"

using namespace std::chrono_literals;

class DrawConflictResolverTest : public testing::Test
{
protected:
    IDrawConflictResolver* m_resolver;

    void SetUp() override
    {
        m_resolver = create_resolver();
    }

    void TearDown() override
    {
        delete m_resolver;
    }

    virtual IDrawConflictResolver* create_resolver() const = 0;
};

namespace time_based_resolver
{
// Test case for resolver that sorts the input by time
class TimeBasedDrawConflictResolverTest : public DrawConflictResolverTest
{
protected:
    IDrawConflictResolver* create_resolver() const override
    {
        // Create a new instance of the time-based resolver
        return new IDrawConflictResolver();
    }
};

TEST_F(TimeBasedDrawConflictResolverTest, EmptyInput)
{
    std::vector<DrawAction> expected;
    auto actual = m_resolver->resolve(expected);

    EXPECT_EQ(actual.size(), 0);
}

TEST_F(TimeBasedDrawConflictResolverTest, SingleActionInput)
{
    std::vector<DrawAction> expected{{ResolverActionType::kInsertion, "figure1", "endpoint1",
                                      std::chrono::system_clock::now(), nullptr}};
    auto actual = m_resolver->resolve(expected);

    EXPECT_EQ(actual[0], expected[0]);
}

TEST_F(TimeBasedDrawConflictResolverTest, MultipleInsertionsOneClientOneFigure)
{
    std::vector<DrawAction> input = {{ResolverActionType::kInsertion, "figure1", "endpoint1",
                                      std::chrono::system_clock::now() - 10s, nullptr},
                                     {ResolverActionType::kInsertion, "figure1", "endpoint1",
                                      std::chrono::system_clock::now(), nullptr},
                                     {ResolverActionType::kInsertion, "figure1", "endpoint1",
                                      std::chrono::system_clock::now() - 5s, nullptr}};

    std::vector<DrawAction> expected = input;
    std::sort(expected.begin(), expected.end(),
              [](const DrawAction& a, const DrawAction& b) { return a.time < b.time; });

    auto actual = m_resolver->resolve(input);

    ASSERT_EQ(actual.size(), expected.size());
    for (size_t i = 0; i < actual.size(); ++i)
    {
        EXPECT_EQ(actual[i], expected[i]);
    }
}

TEST_F(TimeBasedDrawConflictResolverTest, MultipleActionTypesOneClientOneFigure)
{
    std::vector<DrawAction> input = {{ResolverActionType::kInsertion, "figure1", "endpoint1",
                                      std::chrono::system_clock::now() - 10s, nullptr},
                                     {ResolverActionType::kFormat, "figure1", "endpoint1",
                                      std::chrono::system_clock::now(), nullptr},
                                     {ResolverActionType::kSelect, "figure1", "endpoint1",
                                      std::chrono::system_clock::now() - 5s, nullptr}};

    std::vector<DrawAction> expected = input;
    std::sort(expected.begin(), expected.end(),
              [](const DrawAction& a, const DrawAction& b) { return a.time < b.time; });

    auto actual = m_resolver->resolve(input);

    ASSERT_EQ(actual.size(), expected.size());
    for (size_t i = 0; i < actual.size(); ++i)
    {
        EXPECT_EQ(actual[i], expected[i]);
    }
}

TEST_F(TimeBasedDrawConflictResolverTest, TwoClientsOneFigure)
{
    std::vector<DrawAction> input = {{ResolverActionType::kInsertion, "figure1", "endpoint1",
                                      std::chrono::system_clock::now() - 15s, nullptr},
                                     {ResolverActionType::kInsertion, "figure1", "endpoint2",
                                      std::chrono::system_clock::now() - 10s, nullptr},
                                     {ResolverActionType::kInsertion, "figure1", "endpoint1",
                                      std::chrono::system_clock::now(), nullptr},
                                     {ResolverActionType::kInsertion, "figure1", "endpoint1",
                                      std::chrono::system_clock::now() - 5s, nullptr}};

    std::vector<DrawAction> expected = input;
    std::sort(expected.begin(), expected.end(),
              [](const DrawAction& a, const DrawAction& b) { return a.time < b.time; });

    auto actual = m_resolver->resolve(input);

    ASSERT_EQ(actual.size(), expected.size());
    for (size_t i = 0; i < actual.size(); ++i)
    {
        EXPECT_EQ(actual[i], expected[i]);
    }
}

TEST_F(TimeBasedDrawConflictResolverTest, DeletionInMiddle)
{
    std::vector<DrawAction> input = {{ResolverActionType::kInsertion, "figure1", "endpoint1",
                                      std::chrono::system_clock::now() - 10s, nullptr},
                                     {ResolverActionType::kFormat, "figure1", "endpoint1",
                                      std::chrono::system_clock::now() - 5s, nullptr},
                                     {ResolverActionType::kDeletion, "figure1", "endpoint2",
                                      std::chrono::system_clock::now() - 3s, nullptr},
                                     {ResolverActionType::kInsertion, "figure1", "endpoint1",
                                      std::chrono::system_clock::now(), nullptr}};

    std::vector<DrawAction> expected = {{ResolverActionType::kInsertion, "figure1", "endpoint1",
                                         std::chrono::system_clock::now() - 10s, nullptr},
                                        {ResolverActionType::kFormat, "figure1", "endpoint1",
                                         std::chrono::system_clock::now() - 5s, nullptr},
                                        {ResolverActionType::kDeletion, "figure1", "endpoint2",
                                         std::chrono::system_clock::now() - 3s, nullptr}};

    std::vector<DrawAction> actual = m_resolver->resolve(input);

    ASSERT_EQ(actual.size(), expected.size());
    for (size_t i = 0; i < actual.size(); ++i)
    {
        EXPECT_EQ(actual[i], expected[i]);
    }
}

TEST_F(TimeBasedDrawConflictResolverTest, DeletionAfterOtherSelected)
{
    std::vector<DrawAction> input = {
            {ResolverActionType::kSelect, "figure1", "endpoint1",
             std::chrono::system_clock::now() - 10s, nullptr},
            {ResolverActionType::kDeletion, "figure1", "endpoint2",
             std::chrono::system_clock::now() - 3s, nullptr},
    };

    std::vector<DrawAction> expected = input;

    std::vector<DrawAction> actual = m_resolver->resolve(input);

    ASSERT_EQ(actual.size(), expected.size());
    for (size_t i = 0; i < actual.size(); ++i)
    {
        EXPECT_EQ(actual[i], expected[i]);
    }
}

TEST_F(TimeBasedDrawConflictResolverTest, OperationsOnDifferentFigures)
{
    std::vector<DrawAction> input = {{ResolverActionType::kInsertion, "figure1", "endpoint1",
                                      std::chrono::system_clock::now() - 10s, nullptr},
                                     {ResolverActionType::kDeletion, "figure2", "endpoint1",
                                      std::chrono::system_clock::now() - 5s, nullptr},
                                     {ResolverActionType::kFormat, "figure3", "endpoint1",
                                      std::chrono::system_clock::now() - 9s, nullptr},
                                     {ResolverActionType::kSelect, "figure4", "endpoint1",
                                      std::chrono::system_clock::now(), nullptr}};

    // Expected result is the same as the input, as there are no conflicts to resolve
    std::vector<DrawAction> expected = input;

    // Resolve conflicts
    std::vector<DrawAction> actual = m_resolver->resolve(input);

    // Verify the resolved input match the expected result
    ASSERT_EQ(actual.size(), expected.size());
    for (size_t i = 0; i < actual.size(); ++i)
    {
        EXPECT_EQ(actual[i], expected[i]);
    }
}

}  // namespace time_based_resolver

namespace blocking_resolver
{
// Test case for resolver that allows only one user to modify figure
// Blocking starts when user selects figure and ends when he deselects it
// therefore input must not be ill-formated (selection action should be passed each time if any)
class BlockingDrawConflictResolverTest : public DrawConflictResolverTest
{
protected:
    IDrawConflictResolver* create_resolver() const override
    {
        // Create a new instance of the blocking resolver
        return new IDrawConflictResolver();
    }
};

TEST_F(BlockingDrawConflictResolverTest, EmptyInput)
{
    std::vector<DrawAction> expected;
    auto actual = m_resolver->resolve(expected);

    EXPECT_EQ(actual.size(), 0);
}

TEST_F(BlockingDrawConflictResolverTest, NoSelectionInput)
{
    std::vector<DrawAction> input{{ResolverActionType::kInsertion, "figure1", "endpoint1",
                                   std::chrono::system_clock::now(), nullptr},
                                  {ResolverActionType::kInsertion, "figure1", "endpoint1",
                                   std::chrono::system_clock::now() - 5s, nullptr}};

    // result should be empty because there is no selection for endpoint1
    std::vector<DrawAction> expected;
    auto actual = m_resolver->resolve(input);

    EXPECT_EQ(actual.size(), expected.size());
}

TEST_F(BlockingDrawConflictResolverTest, OneUserGoodOtherNoSelectionDifferentFigures)
{
    std::vector<DrawAction> input{
            {ResolverActionType::kSelect, "figure1", "endpoint1",
             std::chrono::system_clock::now() - 5s, nullptr},
            {ResolverActionType::kInsertion, "figureOther", "endpointNoSelection",
             std::chrono::system_clock::now() - 3s, nullptr},
            {ResolverActionType::kInsertion, "figure1", "endpoint1",
             std::chrono::system_clock::now(), nullptr}};

    std::vector<DrawAction> expected{{ResolverActionType::kSelect, "figure1", "endpoint1",
                                      std::chrono::system_clock::now() - 5s, nullptr},
                                     {ResolverActionType::kInsertion, "figure1", "endpoint1",
                                      std::chrono::system_clock::now(), nullptr}};
    auto actual = m_resolver->resolve(input);

    ASSERT_EQ(actual.size(), expected.size());
    for (size_t i = 0; i < actual.size(); ++i)
    {
        EXPECT_EQ(actual[i], expected[i]);
    }
}

TEST_F(BlockingDrawConflictResolverTest, OneUserGoodOtherNoSelectionOneFigure)
{
    std::vector<DrawAction> input{{ResolverActionType::kSelect, "figure1", "endpoint1",
                                   std::chrono::system_clock::now() - 5s, nullptr},
                                  {ResolverActionType::kInsertion, "figure1", "endpointNoSelection",
                                   std::chrono::system_clock::now() - 3s, nullptr},
                                  {ResolverActionType::kInsertion, "figure1", "endpoint1",
                                   std::chrono::system_clock::now(), nullptr}};

    std::vector<DrawAction> expected{{ResolverActionType::kSelect, "figure1", "endpoint1",
                                      std::chrono::system_clock::now() - 5s, nullptr},
                                     {ResolverActionType::kInsertion, "figure1", "endpoint1",
                                      std::chrono::system_clock::now(), nullptr}};
    auto actual = m_resolver->resolve(input);

    ASSERT_EQ(actual.size(), expected.size());
    for (size_t i = 0; i < actual.size(); ++i)
    {
        EXPECT_EQ(actual[i], expected[i]);
    }
}

TEST_F(BlockingDrawConflictResolverTest, NoDeselectSoFar)
{
    std::vector<DrawAction> input = {{ResolverActionType::kSelect, "figure1", "endpoint1",
                                      std::chrono::system_clock::now() - 10s, nullptr},
                                     {ResolverActionType::kInsertion, "figure1", "endpoint1",
                                      std::chrono::system_clock::now() - 5s, nullptr},
                                     {ResolverActionType::kFormat, "figure1", "endpoint1",
                                      std::chrono::system_clock::now() - 3s, nullptr},
                                     {ResolverActionType::kInsertion, "figure1", "endpoint1",
                                      std::chrono::system_clock::now(), nullptr}};

    std::vector<DrawAction> expected = input;

    std::vector<DrawAction> actual = m_resolver->resolve(input);

    ASSERT_EQ(actual.size(), expected.size());
    for (size_t i = 0; i < actual.size(); ++i)
    {
        EXPECT_EQ(actual[i], expected[i]);
    }
}

TEST_F(BlockingDrawConflictResolverTest, FullCycle)
{
    std::vector<DrawAction> input = {{ResolverActionType::kSelect, "figure1", "endpoint1",
                                      std::chrono::system_clock::now() - 10s, nullptr},
                                     {ResolverActionType::kInsertion, "figure1", "endpoint1",
                                      std::chrono::system_clock::now() - 5s, nullptr},
                                     {ResolverActionType::kFormat, "figure1", "endpoint1",
                                      std::chrono::system_clock::now() - 3s, nullptr},
                                     {ResolverActionType::kDeselect, "figure1", "endpoint1",
                                      std::chrono::system_clock::now(), nullptr}};

    std::vector<DrawAction> expected = input;

    std::vector<DrawAction> actual = m_resolver->resolve(input);

    ASSERT_EQ(actual.size(), expected.size());
    for (size_t i = 0; i < actual.size(); ++i)
    {
        EXPECT_EQ(actual[i], expected[i]);
    }
}

TEST_F(BlockingDrawConflictResolverTest, MultipleSelections)
{
    std::vector<DrawAction> input = {{ResolverActionType::kSelect, "figure1", "endpoint1",
                                      std::chrono::system_clock::now() - 10s, nullptr},
                                     {ResolverActionType::kSelect, "figure1", "endpoint1",
                                      std::chrono::system_clock::now() - 5s, nullptr},
                                     {ResolverActionType::kFormat, "figure1", "endpoint1",
                                      std::chrono::system_clock::now() - 3s, nullptr},
                                     {ResolverActionType::kDeselect, "figure1", "endpoint1",
                                      std::chrono::system_clock::now(), nullptr}};

    std::vector<DrawAction> expected = {{ResolverActionType::kSelect, "figure1", "endpoint1",
                                         std::chrono::system_clock::now() - 10s, nullptr},
                                        {ResolverActionType::kFormat, "figure1", "endpoint1",
                                         std::chrono::system_clock::now() - 3s, nullptr},
                                        {ResolverActionType::kDeselect, "figure1", "endpoint1",
                                         std::chrono::system_clock::now(), nullptr}};

    std::vector<DrawAction> actual = m_resolver->resolve(input);

    ASSERT_EQ(actual.size(), expected.size());
    for (size_t i = 0; i < actual.size(); ++i)
    {
        EXPECT_EQ(actual[i], expected[i]);
    }
}

TEST_F(BlockingDrawConflictResolverTest, OtherUserTriesSelectWhenBlocked)
{
    std::vector<DrawAction> input = {{ResolverActionType::kSelect, "figure1", "endpoint1",
                                      std::chrono::system_clock::now() - 10s, nullptr},
                                     {ResolverActionType::kSelect, "figure1", "endpoint2",
                                      std::chrono::system_clock::now() - 5s, nullptr},
                                     {ResolverActionType::kFormat, "figure1", "endpoint1",
                                      std::chrono::system_clock::now() - 3s, nullptr}};

    std::vector<DrawAction> expected = {{ResolverActionType::kSelect, "figure1", "endpoint1",
                                         std::chrono::system_clock::now() - 10s, nullptr},
                                        {ResolverActionType::kFormat, "figure1", "endpoint1",
                                         std::chrono::system_clock::now() - 3s, nullptr}};

    std::vector<DrawAction> actual = m_resolver->resolve(input);

    ASSERT_EQ(actual.size(), expected.size());
    for (size_t i = 0; i < actual.size(); ++i)
    {
        EXPECT_EQ(actual[i], expected[i]);
    }
}

TEST_F(BlockingDrawConflictResolverTest, OtherUserSelectsAfterDeselect)
{
    std::vector<DrawAction> input = {{ResolverActionType::kSelect, "figure1", "endpoint1",
                                      std::chrono::system_clock::now() - 10s, nullptr},
                                     {ResolverActionType::kFormat, "figure1", "endpoint1",
                                      std::chrono::system_clock::now() - 3s, nullptr},
                                     {ResolverActionType::kDeselect, "figure1", "endpoint1",
                                      std::chrono::system_clock::now() - 1s, nullptr},
                                     {ResolverActionType::kSelect, "figure1", "endpoint2",
                                      std::chrono::system_clock::now(), nullptr}};

    std::vector<DrawAction> expected = input;

    std::vector<DrawAction> actual = m_resolver->resolve(input);

    ASSERT_EQ(actual.size(), expected.size());
    for (size_t i = 0; i < actual.size(); ++i)
    {
        EXPECT_EQ(actual[i], expected[i]);
    }
}

TEST_F(BlockingDrawConflictResolverTest, UsersSelectsDifferentFiguresIsOk)
{
    std::vector<DrawAction> input = {{ResolverActionType::kSelect, "figure1", "endpoint1",
                                      std::chrono::system_clock::now() - 10s, nullptr},
                                     {ResolverActionType::kSelect, "figure2", "endpoint2",
                                      std::chrono::system_clock::now() - 5s, nullptr},
                                     {ResolverActionType::kFormat, "figure1", "endpoint1",
                                      std::chrono::system_clock::now() - 3s, nullptr},
                                     {ResolverActionType::kFormat, "figure2", "endpoint2",
                                      std::chrono::system_clock::now(), nullptr}};

    std::vector<DrawAction> expected = input;

    std::vector<DrawAction> actual = m_resolver->resolve(input);

    ASSERT_EQ(actual.size(), expected.size());
    for (size_t i = 0; i < actual.size(); ++i)
    {
        EXPECT_EQ(actual[i], expected[i]);
    }
}

}  // namespace blocking_resolver
