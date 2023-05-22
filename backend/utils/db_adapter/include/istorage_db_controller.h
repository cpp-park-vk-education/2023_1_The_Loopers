#pragma once

#include "idb_adapter.h"

#include <filesystem>
#include <string>

namespace inklink::db_controller
{
class IStorageDbController final
{
public:
    virtual explicit void SetAdapter(DbAdapterBase& adapter) = 0;

    virtual void Connect(const std::string& connectionString) = 0;

    virtual [[nodiscard]] std::filesystem::path GetFilePath(const std::string& fileName,
                                                            const std::string& login) const = 0;

    // root - is the file that created session
    virtual [[nodiscard]] std::string GetGraphArcs(const std::string& rootFileName, const std::string& vertexFileName,
                                                   const std::string& login) const = 0;
    virtual [[nodiscard]] std::string GetAllFilesForUser(std::string& login) const = 0;

    virtual void InsertFile(const std::string& fileName, const std::string& login,
                            const std::filesystem::path& filePath) const = 0;
    virtual void InsertGraphArc(const std::string& rootFileName, const std::string& fromFileName,
                                const std::string& toFileName, const std::string& sessionId) const = 0;

    virtual void SetFileDeleted(const std::string& fileName, const std::string& login) const = 0;

private:
    DbAdapterBase& m_adapter;
};
} // namespace inklink::db_controller
