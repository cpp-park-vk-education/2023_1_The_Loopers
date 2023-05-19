#pragma once

#include "idb_adapter.h"

#include <string>
#include <filesystem>
#include <experimental/filesystem>


namespace inklink::db_controller
{
using filesystem::path = std::experimental::filesystem::path;


class StorageDbController
{
public:
    explicit void SetAdapter(DbAdapterBase& adapter);

    [[nodiscard]] filesystem::path GetFilePath(std::string& fileName, std::string& login) const;
    [[nodiscard]] std::string GetGraphArcs(std::string& vertexName, std::string& login) const;

    void InsertFile(std::string& fileName, std::string& login, filesystem::path& filePath) const;
    void InsertGraphArc(std::string& fromFileName, std::string& toFileName, std::string& login) const;

    void Connect(const std::string& connectionString);

private:
    DbAdapterBase& m_adapter;
};
} // namespace inklink::db_controller
