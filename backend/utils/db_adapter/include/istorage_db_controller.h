#pragma once

#include "storage_db_adapter.h"

#include <filesystem>
#include <memory>

namespace inklink::db_controller
{
class IStorageDbController
{
public:
    using DbRow = std::vector<std::string>;
    using DbTable = std::vector<DbRow>;
    using StorageDbAdapter = db_adapter::StorageDbAdapter;

public:
    virtual ~IStorageDbController() = 0;

    virtual void SetAdapter(std::shared_ptr<StorageDbAdapter> adapter) = 0;

    virtual void Run(const std::string& connectionString) = 0;

    [[nodiscard]] virtual std::filesystem::path GetFilePath(const std::string& fileName,
                                                            const std::string& login) const = 0;

    // root - is the file that created session
    [[nodiscard]] virtual std::string GetGraphArcs(const std::string& rootFileName, const std::string& vertexFileName,
                                                   const std::string& login) const = 0;

    [[nodiscard]] virtual std::string GetAllFilesForUser(const std::string& login) const = 0;

    virtual void InsertRootFile(const std::string& fileName, const std::string& login,
                                const std::filesystem::path& filePath) const = 0;

    virtual void InsertGraphArc(const std::string& login, const std::string& rootFileName,
                                const std::string& fromFileName, const std::string& toFileName) const = 0;

    virtual void InsertNonRootFile(const std::string& fileName, const std::string& login,
                                   const std::filesystem::path& filePath) const = 0;

    virtual void SetFileDeleted(const std::string& fileName, const std::string& login) const = 0;

protected:
    std::shared_ptr<StorageDbAdapter> m_adapter;
};
} // namespace inklink::db_controller
