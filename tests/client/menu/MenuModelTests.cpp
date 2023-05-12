#include "../include/MenuModel.h"

#include <gtest/gtest.h>

TEST(MenuModelTest, ShareLinkTest) {
    MenuModel model;
    model.setShareLink("sharelink");

    EXPECT_EQ("sharelink", model.getShareLink());
}

TEST(MenuModelTest, FileListTest) {
    MenuModel model;
    std::vector<std::string> fileList {"file1.txt", "file2.txt", "file3.txt"};
    model.setFileList(fileList);

    EXPECT_EQ(fileList, model.getFileList());
}

TEST(MenuModelTest, AddFileToListTest) {
    MenuModel model;
    model.addFileToList("file1.txt");
    model.addFileToList("file2.txt");

    std::vector<std::string> expectedFileList {"file1.txt", "file2.txt"};
    EXPECT_EQ(expectedFileList, model.getFileList());
}