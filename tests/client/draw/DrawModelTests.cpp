#pragma once

#include "../include/DrawModel.h"
#include "../include/IObject.h"

#include <string>
#include <vector>

#include <gtest/gtest.h>

TEST(DrawModelTest, AddPolygonObject) {
    DrawModel drawModel;
    std::vector<Point> vertices{{0, 0},
                                {0, 1},
                                {1, 0},
                                {1, 1}};
    drawModel.addObject(1, 4, vertices);
    ASSERT_EQ(drawModel.m_objects.size(), 1);
    ASSERT_EQ(dynamic_cast<Polygon *>(drawModel.m_objects[0])->getID(), 1);
}

TEST(DrawModelTest, AddEllipseObject) {
    DrawModel drawModel;
    drawModel.addObject(2, {0, 0}, 2, 1);
    ASSERT_EQ(drawModel.m_objects.size(), 1);
    ASSERT_EQ(dynamic_cast<Ellipse *>(drawModel.m_objects[0])->getID(), 2);
}

TEST(TextBoxTest, Serialize) {
    TextBox textBox;
    textBox.setID(3);
    textBox.setTextContent("Hello, world!");
    textBox.setTopLeftCorner({10, 20});
    textBox.setWidth(100);
    std::string expected = R"({"id":3,"type":"textbox","text":"Hello, world!","position":{"x":10,"y":20},"width":100})";
    ASSERT_EQ(textBox.serialize(), expected);
}

TEST(PolygonTest, Serialize) {
    Polygon polygon;
    polygon.setID(4);
    polygon.setNumberOfVertex(3);
    polygon.setArrayOfVertexCoordinates({{0, 0},
                                         {1, 0},
                                         {0, 1}});
    std::string expected = R"({"id":4,"type":"polygon","vertices":[{"x":0,"y":0},{"x":1,"y":0},{"x":0,"y":1}]})";
    ASSERT_EQ(polygon.serialize(), expected);
}

TEST(EllipseTest, Serialize) {
    Ellipse ellipse;
    ellipse.setID(5);
    ellipse.setCenter({10, 20});
    ellipse.setXRadius(30);
    ellipse.setYRadius(20);
    std::string expected = R"({"id":5,"type":"ellipse","center":{"x":10,"y":20},"xradius":30,"yradius":20})";
    ASSERT_EQ(ellipse.serialize(), expected);
}
