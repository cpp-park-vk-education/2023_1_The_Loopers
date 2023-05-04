#include <chrono>
#include <vector>

#include "IDrawConflictResolver.h"
#include "global.h"
#include "gtest/gtest.h"

using namespace std::chrono_literals;

// Test fixture to be used in the tests
class DrawConflictResolverTest : public testing::Test
{
  protected:
    IDrawConflictResolver* resolver_;
    IData* data_{nullptr};
    std::vector<DrawAction> input_;
    std::vector<DrawAction> expected_output_;

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

        input_ = {{ResolverActionType::kInsertion, std::chrono::system_clock::now() - 10s, data_},
                  {ResolverActionType::kInsertion, std::chrono::system_clock::now(), data_},
                  {ResolverActionType::kInsertion, std::chrono::system_clock::now() - 5s, data_}};

        // Initialize the expected output vector with the input vector sorted by time
        expected_output_ = input_;
        std::sort(expected_output_.begin(), expected_output_.end(),
                  [](const DrawAction& a, const DrawAction& b)
                  {
                      return a.time < b.time;
                  });
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
    std::vector<DrawAction> input;
    auto output = resolver_->resolve(input);
    EXPECT_EQ(output.size(), 0);
}

// Test case for vector with one element
TEST_F(TimeBasedDrawConflictResolverTest, TestSingleElementVector)
{
    std::vector<DrawAction> input{
            {ResolverActionType::kInsertion, std::chrono::system_clock::now(), data_}};
    auto output = resolver_->resolve(input);
    EXPECT_EQ(output[0], input[0]);
}

// Test case for vector with multiple elements
TEST_F(TimeBasedDrawConflictResolverTest, TestMultipleElementsVector)
{
    auto output = resolver_->resolve(input_);
    EXPECT_EQ(output[0], expected_output_[0]);
}
