#include "blocking_draw_resolver.h"
#include "inklink_global.h"

#include <data_container.h>

#include <gtest/gtest.h>

#include <chrono>
// #include <format>
#include <vector>

// TODO (a.novak) operator << overload for DrawAction

using namespace inklink;
using namespace inklink::service_simultaneous_access;

using namespace std::chrono_literals;

using DataContainer = inklink::serializer::DataContainer;

bool PredBlocking(const DrawAction& lhs, const DrawAction& rhs)
{
    return lhs == rhs;
}

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
    auto input = expected;
    m_Resolver->Resolve(input);

    EXPECT_EQ(input.size(), 0);
}

TEST_F(BlockingDrawConflictResolverTest, NoSelectionInput)
{
    std::vector<DrawAction> input{
            {ResolverActionType::kInsertion, "figure1", endpoint1, std::chrono::system_clock::now(), DataContainer()},
            {ResolverActionType::kInsertion, "figure1", endpoint1, std::chrono::system_clock::now() - 5s,
             DataContainer()}};

    // result should be empty because there is no selection for endpoint1
    std::vector<DrawAction> expected;
    m_Resolver->Resolve(input);

    EXPECT_EQ(input.size(), expected.size());
}

TEST_F(BlockingDrawConflictResolverTest, OneUserGoodOtherNoSelectionDifferentFigures)
{
    std::vector<DrawAction> input{
            {ResolverActionType::kSelect, "figure1", endpoint1, std::chrono::system_clock::now() - 5s, DataContainer()},
            {ResolverActionType::kInsertion, "figureOther", endpoint2, std::chrono::system_clock::now() - 3s,
             DataContainer()},
            {ResolverActionType::kInsertion, "figure1", endpoint1, std::chrono::system_clock::now(), DataContainer()}};

    std::vector<DrawAction> expected{input[0], input[2]};
    m_Resolver->Resolve(input);

    ASSERT_EQ(input.size(), expected.size());
    for (size_t i = 0; i < input.size(); ++i)
    {
        EXPECT_EQ(input[i], expected[i]) << "Actual type " << (int)input[i].type << " figure " << input[i].figureId
                                         << " endpoint " << input[i].endpoint.address << " time "
                                         << input[i].time.time_since_epoch().count() << " data ptr "
                                         << "\nExpected type " << (int)expected[i].type << " figure "
                                         << expected[i].figureId << " endpoint " << expected[i].endpoint.address
                                         << " time " << expected[i].time.time_since_epoch().count() << " data ptr ";
    }
}

TEST_F(BlockingDrawConflictResolverTest, OneUserGoodOtherNoSelectionOneFigure)
{
    std::vector<DrawAction> input{
            {ResolverActionType::kSelect, "figure1", endpoint1, std::chrono::system_clock::now() - 5s, DataContainer()},
            {ResolverActionType::kInsertion, "figure1", endpoint2, std::chrono::system_clock::now() - 3s,
             DataContainer()},
            {ResolverActionType::kInsertion, "figure1", endpoint1, std::chrono::system_clock::now(), DataContainer()}};

    std::vector<DrawAction> expected{input[0], input[2]};
    m_Resolver->Resolve(input);

    ASSERT_EQ(input.size(), expected.size());
    for (size_t i = 0; i < input.size(); ++i)
    {
        EXPECT_EQ(input[i], expected[i]) << "Actual type " << (int)input[i].type << " figure " << input[i].figureId
                                         << " endpoint " << input[i].endpoint.address << " time "
                                         << input[i].time.time_since_epoch().count() << " data ptr "
                                         << "\nExpected type " << (int)expected[i].type << " figure "
                                         << expected[i].figureId << " endpoint " << expected[i].endpoint.address
                                         << " time " << expected[i].time.time_since_epoch().count() << " data ptr ";
    }
}

TEST_F(BlockingDrawConflictResolverTest, NoDeselectSoFar)
{
    std::vector<DrawAction> input = {
            {ResolverActionType::kSelect, "figure1", endpoint1, std::chrono::system_clock::now() - 10s,
             DataContainer()},
            {ResolverActionType::kInsertion, "figure1", endpoint1, std::chrono::system_clock::now() - 5s,
             DataContainer()},
            {ResolverActionType::kFormat, "figure1", endpoint1, std::chrono::system_clock::now() - 3s, DataContainer()},
            {ResolverActionType::kInsertion, "figure1", endpoint1, std::chrono::system_clock::now(), DataContainer()}};

    std::vector<DrawAction> expected = input;

    m_Resolver->Resolve(input);

    ASSERT_EQ(input.size(), expected.size());
    for (size_t i = 0; i < input.size(); ++i)
    {
        EXPECT_EQ(input[i], expected[i]) << "Actual type " << (int)input[i].type << " figure " << input[i].figureId
                                         << " endpoint " << input[i].endpoint.address << " time "
                                         << input[i].time.time_since_epoch().count() << " data ptr "
                                         << "\nExpected type " << (int)expected[i].type << " figure "
                                         << expected[i].figureId << " endpoint " << expected[i].endpoint.address
                                         << " time " << expected[i].time.time_since_epoch().count() << " data ptr ";
    }
}

TEST_F(BlockingDrawConflictResolverTest, FullCycle)
{
    std::vector<DrawAction> input = {
            {ResolverActionType::kSelect, "figure1", endpoint1, std::chrono::system_clock::now() - 10s,
             DataContainer()},
            {ResolverActionType::kInsertion, "figure1", endpoint1, std::chrono::system_clock::now() - 5s,
             DataContainer()},
            {ResolverActionType::kFormat, "figure1", endpoint1, std::chrono::system_clock::now() - 3s, DataContainer()},
            {ResolverActionType::kDeselect, "figure1", endpoint1, std::chrono::system_clock::now(), DataContainer()}};

    std::vector<DrawAction> expected = input;

    m_Resolver->Resolve(input);

    ASSERT_EQ(input.size(), expected.size());
    for (size_t i = 0; i < input.size(); ++i)
    {
        EXPECT_EQ(input[i], expected[i]) << "Actual type " << (int)input[i].type << " figure " << input[i].figureId
                                         << " endpoint " << input[i].endpoint.address << " time "
                                         << input[i].time.time_since_epoch().count() << " data ptr "
                                         << "\nExpected type " << (int)expected[i].type << " figure "
                                         << expected[i].figureId << " endpoint " << expected[i].endpoint.address
                                         << " time " << expected[i].time.time_since_epoch().count() << " data ptr ";
    }
}

TEST_F(BlockingDrawConflictResolverTest, MultipleSelections)
{
    std::vector<DrawAction> input = {
            {ResolverActionType::kSelect, "figure1", endpoint1, std::chrono::system_clock::now() - 10s,
             DataContainer()},
            {ResolverActionType::kSelect, "figure1", endpoint2, std::chrono::system_clock::now() - 5s, DataContainer()},
            {ResolverActionType::kFormat, "figure1", endpoint1, std::chrono::system_clock::now() - 3s, DataContainer()},
            {ResolverActionType::kDeselect, "figure1", endpoint1, std::chrono::system_clock::now(), DataContainer()}};

    std::vector<DrawAction> expected = {input[0], input[2], input[3]};

    m_Resolver->Resolve(input);

    ASSERT_EQ(input.size(), expected.size());
    for (size_t i = 0; i < input.size(); ++i)
    {
        EXPECT_EQ(input[i], expected[i]) << "Actual type " << (int)input[i].type << " figure " << input[i].figureId
                                         << " endpoint " << input[i].endpoint.address << " time "
                                         << input[i].time.time_since_epoch().count() << " data ptr "
                                         << "\nExpected type " << (int)expected[i].type << " figure "
                                         << expected[i].figureId << " endpoint " << expected[i].endpoint.address
                                         << " time " << expected[i].time.time_since_epoch().count() << " data ptr ";
    }
}

TEST_F(BlockingDrawConflictResolverTest, OtherUserTriesSelectWhenBlocked)
{
    std::vector<DrawAction> input = {
            {ResolverActionType::kSelect, "figure1", endpoint1, std::chrono::system_clock::now() - 10s,
             DataContainer()},
            {ResolverActionType::kSelect, "figure1", endpoint2, std::chrono::system_clock::now() - 5s, DataContainer()},
            {ResolverActionType::kFormat, "figure1", endpoint1, std::chrono::system_clock::now() - 3s,
             DataContainer()}};

    std::vector<DrawAction> expected = {input[0], input[2]};

    m_Resolver->Resolve(input);

    ASSERT_EQ(input.size(), expected.size());
    for (size_t i = 0; i < input.size(); ++i)
    {
        EXPECT_EQ(input[i], expected[i]) << "Actual type " << (int)input[i].type << " figure " << input[i].figureId
                                         << " endpoint " << input[i].endpoint.address << " time "
                                         << input[i].time.time_since_epoch().count() << " data ptr "
                                         << "\nExpected type " << (int)expected[i].type << " figure "
                                         << expected[i].figureId << " endpoint " << expected[i].endpoint.address
                                         << " time " << expected[i].time.time_since_epoch().count() << " data ptr ";
    }
}

TEST_F(BlockingDrawConflictResolverTest, OtherUserSelectsAfterDeselect)
{
    std::vector<DrawAction> input = {
            {ResolverActionType::kSelect, "figure1", endpoint1, std::chrono::system_clock::now() - 10s,
             DataContainer()},
            {ResolverActionType::kFormat, "figure1", endpoint1, std::chrono::system_clock::now() - 3s, DataContainer()},
            {ResolverActionType::kDeselect, "figure1", endpoint1, std::chrono::system_clock::now() - 1s,
             DataContainer()},
            {ResolverActionType::kSelect, "figure1", endpoint2, std::chrono::system_clock::now(), DataContainer()}};

    std::vector<DrawAction> expected = input;

    m_Resolver->Resolve(input);

    ASSERT_EQ(input.size(), expected.size());
    for (size_t i = 0; i < input.size(); ++i)
    {
        EXPECT_EQ(input[i], expected[i]) << "Actual type " << (int)input[i].type << " figure " << input[i].figureId
                                         << " endpoint " << input[i].endpoint.address << " time "
                                         << input[i].time.time_since_epoch().count() << " data ptr "
                                         << "\nExpected type " << (int)expected[i].type << " figure "
                                         << expected[i].figureId << " endpoint " << expected[i].endpoint.address
                                         << " time " << expected[i].time.time_since_epoch().count() << " data ptr ";
    }
}

TEST_F(BlockingDrawConflictResolverTest, UsersSelectsDifferentFiguresIsOk)
{
    std::vector<DrawAction> input = {
            {ResolverActionType::kSelect, "figure1", endpoint1, std::chrono::system_clock::now() - 10s,
             DataContainer()},
            {ResolverActionType::kSelect, "figure2", endpoint2, std::chrono::system_clock::now() - 5s, DataContainer()},
            {ResolverActionType::kFormat, "figure1", endpoint1, std::chrono::system_clock::now() - 3s, DataContainer()},
            {ResolverActionType::kFormat, "figure2", endpoint2, std::chrono::system_clock::now(), DataContainer()}};

    std::vector<DrawAction> expected = input;

    m_Resolver->Resolve(input);

    ASSERT_EQ(input.size(), expected.size());
    for (size_t i = 0; i < input.size(); ++i)
    {
        EXPECT_EQ(input[i], expected[i]) << "Actual type " << (int)input[i].type << " figure " << input[i].figureId
                                         << " endpoint " << input[i].endpoint.address << " time "
                                         << input[i].time.time_since_epoch().count() << " data ptr "
                                         << "\nExpected type " << (int)expected[i].type << " figure "
                                         << expected[i].figureId << " endpoint " << expected[i].endpoint.address
                                         << " time " << expected[i].time.time_since_epoch().count() << " data ptr ";
    }
}
