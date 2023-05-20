#pragma once

#include "idb_adapter.h"

#include <experimental/filesystem>
#include <filesystem>
#include <string>


namespace inklink::db_controller
{
using filesystem::path = std::experimental::filesystem::path;
using namespace data_types;

class IStorageDbController final
{
public:
    virtual explicit void SetAdapter(DbAdapterBase& adapter) = 0;

    virtual void Connect(const std::string& connectionString) = 0;

    virtual [[nodiscard]] filesystem::path GetFilePath(std::string& fileName, std::string& login) const = 0;
    virtual [[nodiscard]] std::string GetGraphArcs(std::string& vertexName, std::string& sessionId) const = 0;
    virtual [[nodiscard]] std::string GetAllFilesForUser(std::string& login) const = 0;

    virtual void InsertFile(std::string& fileName, std::string& login, filesystem::path& filePath) const = 0;
    virtual void InsertGraphArc(std::string& fromFileName, std::string& toFileName, std::string& login) const = 0;

private:
    DbAdapterBase& m_adapter;
};
} // namespace inklink::db_controller