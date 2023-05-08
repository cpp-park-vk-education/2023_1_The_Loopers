
#include "DB_Adaptor.h"
#include "FileHolder.h"
#include "Storage.h"
#include "gtest/gtest.h"


class DBAdapterStub : public DBAdapter
{
public:
    std::string Select(std::string tableName, std::string login, std::string fileName)
    {
        if (login != "wrongLogin")
        {
            return "filePath";
        }
        if (fileName == "newFile")
        {
            return "newFile";
        }
        return "";
    }

    bool Update(std::string tableName, std::string login, std::string fileName)
    {
        if (login != "wrongLogin")
        {
            return true;
        }
        return false;
    }

    bool Insert(std::string tableName, std::string login, std::string fileName)
    {
        if (login != "wrongLogin")
        {
            return true;
        }
        return false;
    }
};


class FileHolderMock : public FileHolder
{
public:
    bool Save(std::string filePath, std::string fileChanges)
    {
    
    }

    bool Delete(std::string filePath)
    {

    }

    std::string Read(std::string filePath)
    {
        return "FILE";
    }


};




TEST(Storage, GetFile)
{
    Storage storage;
    DBAdapterStub* dbAdapter;
    FileHolderMock* fileHolder;
    storage.SetDbAdaptor(dbAdapter);
    storage.SetFileHolder(fileHolder);
    std::string tableName("tableName");
    std::string login("login");
    std::string fileName("fileName");
    std::string file;
    std::string wrongLogin("wrongLogin");



    file = storage.GetFile(login, fileName);
    ASSERT_EQ(file, "FILE");
    file = storage.GetFile(wrongLogin, fileName);
    ASSERT_EQ(file, "");
}

TEST(Storage, Update)
{
    Storage storage;
    DBAdapterStub* dbAdapter;
    FileHolderMock* fileHolder;
    storage.SetDbAdaptor(dbAdapter);
    storage.SetFileHolder(fileHolder);
    std::string tableName("tableName");
    std::string login("login");
    std::string fileName("fileName");
    std::string wrongLogin("wrongLogin");
    std::string newFile("newFile");
    std::string changes("changes");


    ASSERT_TRUE(storage.Update(login, fileName, changes));
    ASSERT_TRUE(storage.Update(login, newFile, changes));
    ASSERT_FALSE(storage.Update(wrongLogin, fileName, changes));
}