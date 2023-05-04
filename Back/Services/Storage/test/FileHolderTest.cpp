
#include "FileHolder.h"
#include "gtest/gtest.h"


TEST(FileHolder, Save) {
    std::string path1("");
    std::string path2("fileHolderTest.txt");
    std::string changes("SomeChanges");
    FileHolder fileHolder;

    ASSERT_FALSE(fileHolder.Save(path1, changes));
    ASSERT_TRUE(fileHolder.Save(path2, changes));
}


TEST(FileHolder, Read) {
    std::string path1("");
    std::string path2("fileHolderTest.txt");
    std::string excpected("SomeChanges");
    std::string errorMessage("Uncorrect file path");
    FileHolder fileHolder;

    ASSERT_EQ(errorMessage, fileHolder.Read(path1));
    ASSERT_EQ(excpected, fileHolder.Read(path2));
}


TEST(FileHolder, Delete) {
    std::string path1("");
    std::string path2("fileHolderTest.txt");
    FileHolder fileHolder;

    ASSERT_FALSE(fileHolder.Delete(path1));
    ASSERT_TRUE(fileHolder.Delete(path2));
}
