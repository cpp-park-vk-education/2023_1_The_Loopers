#include "../include/GraphModel.h"
#include "../include/IEdge.h"
#include "../include/IVertex.h"

#include <gtest/gtest.h>

/// Graph tests
TEST(GraphModelTest, AddVertexesToModelTest) {
    GraphModel model;
    std::vector<std::string> testVertexes = {"A", "B", "C"};
    model.addVertexesToModel(testVertexes);
    std::vector<IVertex*> uniqueVertexes = model.getUniqueVertexes();
    ASSERT_EQ(uniqueVertexes.size(), 3);
    ASSERT_EQ(uniqueVertexes[0]->getName(), "A");
    ASSERT_EQ(uniqueVertexes[1]->getName(), "B");
    ASSERT_EQ(uniqueVertexes[2]->getName(), "C");
}

TEST(GraphModelTest, GetUniqueVertexesTest) {
    GraphModel model;
    std::vector<std::string> testVertexes  = {"A", "B", "B", "C", "C", "C"};
    model.addVertexesToModel(testVertexes);
    std::vector<IVertex*> vertexes = model.getUniqueVertexes();
    ASSERT_EQ(vertexes.size(), 3);
}

TEST(GraphModelTest, GetEdges) {
    GraphModel model;
    std::vector<std::string> vertex_names = {"A", "B", "C"};
    model.addVertexesToModel(vertex_names);
    IVertex* a = model.getUniqueVertexes()[0];
    IVertex* b = model.getUniqueVertexes()[1];
    IVertex* c = model.getUniqueVertexes()[2];
    a->addEdge(new IEdge(a, b));
    b->addEdge(new IEdge(b, c));
    std::vector<IEdge*> edgeList = model.getEdges();
    ASSERT_EQ(edgeList.size(), 2);
}

/// IEdge tests
//nothing to test
TEST(IEdgeTest, GetSourceVertex) {
    IVertex* a = new IVertex("A");
    IVertex* b = new IVertex("B");
    IEdge* edge = new IEdge(a, b);
    ASSERT_EQ(edge->getSourceVertex(), a);
}

TEST(IEdgeTest, GetDestinationVertex) {
    IVertex* a = new IVertex("A");
    IVertex* b = new IVertex("B");
    IEdge* edge = new IEdge(a, b);
    ASSERT_EQ(edge->getDestinationVertex(), b);
}

/// IVertex tests
TEST(IVertexTest, AdvancePositionIfPostionChangedTest) {
    IVertex *a = new IVertex("A");

    Position nextPosition, currentPosition;
    currentPosition.xPosition = 0;
    currentPosition.yPosition = 0;
    nextPosition.xPosition = 1;
    nextPosition.yPosition = 1;

    a->setCurrentPosition(currentPosition);
    a->setNewPosition(nextPosition)

    Position initialPosition = a->getCurrentPosition();
    a->advancePosition();
    Position newPosition = a->getCurrentPosition();

    ASSERT_TRUE(initialPosition.xPosition != newPosition.xPosition || initialPosition.yPosition != newPosition.yPosition);
}

TEST(IVertexTest, AdvancePositionIfPostionChangedTest) {
    IVertex *a = new IVertex("A");

    Position nextPosition, currentPosition;
    currentPosition.xPosition = 0;
    currentPosition.yPosition = 0;
    nextPosition.xPosition = 0;
    nextPosition.yPosition = 0;

    a->setCurrentPosition(currentPosition);
    a->setNewPosition(nextPosition)

    Position initialPosition = a->getCurrentPosition();
    a->advancePosition();
    Position newPosition = a->getCurrentPosition();

    ASSERT_TRUE(initialPosition.xPosition == newPosition.xPosition && initialPosition.yPosition == newPosition.yPosition);
}

TEST(IVertexTest, GetEdges) {
    IVertex* a = new IVertex("A");
    IVertex* b = new IVertex("B");
    IEdge* edge = new IEdge(a, b);
    a->addEdge(edge);
    std::vector<IEdge*> edgeList = a->getEdges();
    ASSERT_EQ(edgeList.size(), 1);
}