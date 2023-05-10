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

        // m_inputInsertionsOnly = {{ResolverActionType::kInsertion,
        // std::chrono::system_clock::now() - 10s},
        //           {kDeletion, std::chrono::system_clock::now() - 5s},
        //           {kFormat, std::chrono::system_clock::now()},
        //           {kSelect, std::chrono::system_clock::now() + 5s},
        //           {kDeselect, std::chrono::system_clock::now() + 10s}};

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
