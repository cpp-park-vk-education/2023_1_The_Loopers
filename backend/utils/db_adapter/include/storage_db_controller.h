#pragma once

#include "idb_adapter.h"

#include <string>
#include <filesystem>
#include <experimental/filesystem>


namespace inklink::db_controller
{
namespace filesystem = std::experimental::filesystem;


class StorageDbController
{
public:
    explicit void SetAdapter(PgSqlIDbAdapter& adapter);

    filesystem::path GetFilePath(std::string fileName, std::string login);
    std::string GetGraphArcs(std::string vertexName, std::string login);

    void InsertFile(std::string fileName, std::string login, filesystem::path filePath);
    void InsertGraphArc(std::string fromFileName, std::string toFileName, std::string login);

    void Connect(const std::string& connectionString);

private:
    DbAdapterBase& m_adapter;
};
} // namespace inklink::db_controller
