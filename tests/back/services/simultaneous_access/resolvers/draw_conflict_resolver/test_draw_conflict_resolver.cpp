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
    IDrawConflictResolver* resolver_;
    IData* data_{nullptr};
    std::vector<DrawAction> input_;
    std::vector<DrawAction> expected_;

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

        input_ = {{ResolverActionType::kInsertion, "id1", "u1",
                   std::chrono::system_clock::now() - 10s, data_},
                  {ResolverActionType::kInsertion, "id1", "u1", std::chrono::system_clock::now(),
                   data_},
                  {ResolverActionType::kInsertion, "id1", "u1",
                   std::chrono::system_clock::now() - 5s, data_}};

        // Initialize the expected output vector with the input vector sorted by time
        expected_ = input_;
        std::sort(expected_output_.begin(), expected_output_.end(),
                  [](const DrawAction& a, const DrawAction& b) { return a.time < b.time; });
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

// Test case for empty input vector
TEST_F(TimeBasedDrawConflictResolverTest, TestEmptyVector)
{
    std::vector<DrawAction> expected;
    auto actual = resolver_->resolve(expected);

    EXPECT_EQ(actual.size(), 0);
}

// Test case for vector with one element
TEST_F(TimeBasedDrawConflictResolverTest, TestSingleElementVector)
{
    std::vector<DrawAction> expected{
            {ResolverActionType::kInsertion, "id1", "u1", std::chrono::system_clock::now(), data_}};
    auto actual = resolver_->resolve(expected);

    EXPECT_EQ(actual[0], expected[0]);
}

// Test case for vector with multiple elements
TEST_F(TimeBasedDrawConflictResolverTest, TestMultipleElementsVector)
{
    auto actual = resolver_->resolve(input_);

    ASSERT_EQ(actions.size(), expected_.size());
    for (size_t i = 0; i < actions.size(); ++i)
    {
        EXPECT_EQ(actions[i], expected_[i]);
    }
}
