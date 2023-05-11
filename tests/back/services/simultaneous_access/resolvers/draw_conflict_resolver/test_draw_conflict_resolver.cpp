#include <chrono>
#include <vector>

#include "global.h"
#include "gtest/gtest.h"
#include "idraw_conflict_resolver.h"

using namespace std::chrono_literals;

// Test fixture to be used in the tests
class DrawConflictResolverTest : public testing::Test
{
protected:
    IDrawConflictResolver* m_resolver;
    IData* m_data{nullptr};
    std::vector<DrawAction> m_inputInsertionsOnly;
    std::vector<DrawAction> m_expectedInsertionsOnly;

    void SetUp() override
    {
        m_resolver = create_resolver();

        m_inputInsertionsOnly = {{ResolverActionType::kInsertion, "figure1", "endpoint1",
                                  std::chrono::system_clock::now() - 10s, m_data},
                                 {ResolverActionType::kInsertion, "figure1", "endpoint2",
                                  std::chrono::system_clock::now(), m_data},
                                 {ResolverActionType::kInsertion, "figure1", "endpoint1",
                                  std::chrono::system_clock::now() - 5s, m_data}};

        // Initialize the expected output vector with the input vector sorted by time
        m_expectedInsertionsOnly = m_inputInsertionsOnly;
        std::sort(m_expectedInsertionsOnlyoutput_.begin(), m_expectedInsertionsOnlyoutput_.end(),
                  [](const DrawAction& a, const DrawAction& b) { return a.time < b.time; });
    }

    void TearDown() override
    {
        // Destroy the resolver after each test
        delete m_resolver;
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

// Test case for empty input vector
TEST_F(TimeBasedDrawConflictResolverTest, TestEmptyVector)
{
    std::vector<DrawAction> expected;
    auto actual = m_resolver->resolve(expected);

    EXPECT_EQ(actual.size(), 0);
}

// Test case for vector with one element
TEST_F(TimeBasedDrawConflictResolverTest, TestSingleElementVector)
{
    std::vector<DrawAction> expected{{ResolverActionType::kInsertion, "figure1", "endpoint1",
                                      std::chrono::system_clock::now(), m_data}};
    auto actual = m_resolver->resolve(expected);

    EXPECT_EQ(actual[0], expected[0]);
}

// Test case for vector with multiple elements
TEST_F(TimeBasedDrawConflictResolverTest, TestMultipleElementsVector)
{
    auto actual = m_resolver->resolve(m_inputInsertionsOnly);

    ASSERT_EQ(actions.size(), m_expectedInsertionsOnly.size());
    for (size_t i = 0; i < actions.size(); ++i)
    {
        EXPECT_EQ(actions[i], m_expectedInsertionsOnly[i]);
    }
}

// Test case for multiple operation types on the same figure
TEST_F(TimeBasedDrawConflictResolverTest, TestMultipleOperationTypesSameFigure)
{
    std::vector<DrawAction> actions = {
            {ResolverActionType::kInsertion, "figure1", "endpoint1",
             std::chrono::system_clock::now() - 10s, m_data},
            {ResolverActionType::kDeletion, "figure1", "endpoint2",
             std::chrono::system_clock::now() - 5s, m_data},
            {ResolverActionType::kFormat, "figure1", "endpoint1",
             std::chrono::system_clock::now() - 3s, m_data},
            {ResolverActionType::kSelect, "figure1", "endpoint2", std::chrono::system_clock::now(),
             m_data},
    };

    // Expected result is the same as the input, as there are no conflicts to resolve
    std::vector<DrawAction> expected = actions;

    // Resolve conflicts
    std::vector<DrawAction> resolvedActions = m_resolver->resolve(actions);

    // Verify the resolved actions match the expected result
    ASSERT_EQ(resolvedActions.size(), expected.size());
    for (size_t i = 0; i < resolvedActions.size(); ++i)
    {
        EXPECT_EQ(resolvedActions[i], expected[i]);
    }
}

// Test case for multiple endpoints on the same figure
TEST_F(TimeBasedDrawConflictResolverTest, TestMultipleEndpointsSameFigure)
{
    std::vector<DrawAction> actions = {
            {ResolverActionType::kInsertion, "figure1", "endpoint1",
             std::chrono::system_clock::now() - 10s, m_data},
            {ResolverActionType::kInsertion, "figure1", "endpoint2",
             std::chrono::system_clock::now() - 5s, m_data},
            {ResolverActionType::kInsertion, "figure1", "endpoint1",
             std::chrono::system_clock::now() - 3s, m_data},
    };

    // Expected result is the same as the input, as there are no conflicts to resolve
    std::vector<DrawAction> expected = actions;

    // Resolve conflicts
    std::vector<DrawAction> resolvedActions = m_resolver->resolve(actions);

    // Verify the resolved actions match the expected result
    ASSERT_EQ(resolvedActions.size(), expected.size());
    for (size_t i = 0; i < resolvedActions.size(); ++i)
    {
        EXPECT_EQ(resolvedActions[i], expected[i]);
    }
}

// Test case for multiple endpoints on the same figure with figure deletion
TEST_F(TimeBasedDrawConflictResolverTest, TestMultipleEndpointsSameFigureWithDeletion)
{
    std::vector<DrawAction> actions = {
            {ResolverActionType::kInsertion, "figure1", "endpoint1",
             std::chrono::system_clock::now() - 10s, m_data},
            {ResolverActionType::kDeletion, "figure1", "endpoint2",
             std::chrono::system_clock::now() - 5s, m_data},
            {ResolverActionType::kInsertion, "figure1", "endpoint1",
             std::chrono::system_clock::now() - 3s, m_data},
    };

    // Expected result is an empty vector, as the figure is deleted and subsequent insertions are
    // cancelled
    std::vector<DrawAction> expected;

    // Resolve conflicts

    std::vector<DrawAction> resolvedActions = m_resolver->resolve(actions);

    // Verify the resolved actions match the expected result
    ASSERT_EQ(resolvedActions.size(), expected.size());
    for (size_t i = 0; i < resolvedActions.size(); ++i)
    {
        EXPECT_EQ(resolvedActions[i], expected[i]);
    }
}

// Test case for multiple operations on different figures
TEST_F(TimeBasedDrawConflictResolverTest, TestMultipleOperationsDifferentFigures)
{
    std::vector<DrawAction> actions = {
            {ResolverActionType::kInsertion, "figure1", "endpoint1",
             std::chrono::system_clock::now() - 10s, m_data},
            {ResolverActionType::kDeletion, "figure2", "endpoint1",
             std::chrono::system_clock::now() - 5s, m_data},
            {ResolverActionType::kFormat, "figure3", "endpoint1",
             std::chrono::system_clock::now() - 10s, m_data},
            {ResolverActionType::kSelect, "figure4", "endpoint1", std::chrono::system_clock::now(),
             m_data},
    };

    // Expected result is the same as the input, as there are no conflicts to resolve
    std::vector<DrawAction> expected = actions;

    // Resolve conflicts
    std::vector<DrawAction> resolvedActions = m_resolver->resolve(actions);

    // Verify the resolved actions match the expected result
    ASSERT_EQ(resolvedActions.size(), expected.size());
    for (size_t i = 0; i < resolvedActions.size(); ++i)
    {
        EXPECT_EQ(resolvedActions[i], expected[i]);
    }
}

// Test case for resolver that allows only one user to modify figure
class BlockingDrawConflictResolverTest : public DrawConflictResolverTest
{
protected:
    IDrawConflictResolver* create_resolver() const override
    {
        // Create a new instance of the blocking resolver
        return new IDrawConflictResolver();
    }
};
// Blocking starts when user selects figure and ends when he deselects it

// Test case for multiple operation types on the same figure
TEST_F(BlockingDrawConflictResolverTest, TestMultipleOperationTypesSameFigure)
{
    std::vector<DrawAction> actions = {
            {ResolverActionType::kInsertion, "figure1", "endpoint1",
             std::chrono::system_clock::now() - 10s, m_data},
            {ResolverActionType::kDeletion, "figure1", "endpoint2",
             std::chrono::system_clock::now() - 5s, m_data},
            {ResolverActionType::kFormat, "figure1", "endpoint1",
             std::chrono::system_clock::now() - 3s, m_data},
            {ResolverActionType::kSelect, "figure1", "endpoint2", std::chrono::system_clock::now(),
             m_data},
    };

    // Only the last action (kSelect) should be preserved due to blocking
    std::vector<DrawAction> expected = {actions.back()};

    // Resolve conflicts
    std::vector<DrawAction> resolvedActions = m_resolver->resolve(actions);

    // Verify the resolved actions match the expected result
    ASSERT_EQ(resolvedActions.size(), expected.size());
    EXPECT_EQ(resolvedActions[0], expected[0]);
}

// Test case for multiple endpoints on the same figure
TEST_F(BlockingDrawConflictResolverTest, TestMultipleEndpointsSameFigure)
{
    std::vector<DrawAction> actions = {
            {ResolverActionType::kInsertion, "figure1", "endpoint1",
             std::chrono::system_clock::now() - 10s, m_data},
            {ResolverActionType::kInsertion, "figure1", "endpoint2",
             std::chrono::system_clock::now() - 5s, m_data},
            {ResolverActionType::kInsertion, "figure1", "endpoint1",
             std::chrono::system_clock::now(), m_data},
            {ResolverActionType::kSelect, "figure1", "endpoint2",
             std::chrono::system_clock::now() + 5s, m_data},
    };

    // Only the last action (kSelect) should be preserved due to blocking
    std::vector<DrawAction> expected = {actions.back()};

    // Resolve conflicts
    std::vector<DrawAction> resolvedActions = m_resolver->resolve(actions);

    // Verify the resolved actions match the expected result
    ASSERT_EQ(resolvedActions.size(), expected.size());
    EXPECT_EQ(resolvedActions[0], expected[0]);
}

// Test case for multiple operations on different figures
TEST_F(BlockingDrawConflictResolverTest, TestMultipleOperationsDifferentFigures)
{
    std::vector<DrawAction> actions = {
            {ResolverActionType::kInsertion, "figure1", "endpoint1",
             std::chrono::system_clock::now() - 10s, m_data},
            {ResolverActionType::kDeletion, "figure2", "endpoint2",
             std::chrono::system_clock::now() - 5s, m_data},
            {ResolverActionType::kFormat, "figure3", "endpoint3",
             std::chrono::system_clock::now() - 3s, m_data},
            {ResolverActionType::kSelect, "figure4", "endpoint4", std::chrono::system_clock::now(),
             m_data},
    };

    // All actions should be preserved as there are no conflicts between figures
    std::vector<DrawAction> expected = actions;

    // Resolve conflicts
    std::vector<DrawAction> resolvedActions = m_resolver->resolve(actions);

    // Verify the resolved actions match the expected result
    ASSERT_EQ(resolvedActions.size(), expected.size());
    for (size_t i = 0; i < resolvedActions.size(); ++i)
    {
        EXPECT_EQ(resolvedActions[i], expected[i]);
    }
}

// Test case for other necessary scenario (e.g., figure deletion cancels subsequent insertions)
TEST_F(BlockingDrawConflictResolverTest, TestFigureDeletionCancelsInsertions)
{
    std::vector<DrawAction> actions = {
            {ResolverActionType::kInsertion, "figure1", "endpoint1",
             std::chrono::system_clock::now() - 10s, m_data},
            {ResolverActionType::kDeletion, "figure1", "endpoint2",
             std::chrono::system_clock::now() - 5s, m_data},
            {ResolverActionType::kInsertion, "figure1", "endpoint1",
             std::chrono::system_clock::now(), m_data},
            {ResolverActionType::kInsertion, "figure1", "endpoint2",
             std::chrono::system_clock::now() + 5s, m_data},
    };

    // The deletion action should cancel the subsequent insertions on the same figure
    std::vector<DrawAction> expected = {
            actions[0],
            actions[1],
    };

    // Resolve conflicts
    std::vector<DrawAction> resolvedActions = m_resolver->resolve(actions);

    // Verify the resolved actions match the expected result
    ASSERT_EQ(resolvedActions.size(), expected.size());
    for (size_t i = 0; i < resolvedActions.size(); ++i)
    {
        EXPECT_EQ(resolvedActions[i], expected[i]);
    }
}
