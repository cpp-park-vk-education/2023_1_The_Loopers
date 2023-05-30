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
    virtual void SetAdapter(std::shared_ptr<StorageDbAdapter> adapter) = 0;

    virtual void Run(const std::string& connectionString) = 0;

    [[nodiscard]] virtual std::filesystem::path GetFilePath(const std::string& fileName,
                                                            const std::string& login) const = 0;

<<<<<<< HEAD
    // root - is the file that created session
    [[nodiscard]] virtual std::string GetGraphArcs(const std::string& rootFileName, const std::string& vertexFileName,
                                                   const std::string& login) const = 0;
=======
    //root - is the file that created session
    [[nodiscard]] virtual std::string GetGraphArcs(const std::string& rootFileName, const std::string& vertexFileName,
                                                  const std::string& login) const = 0;
>>>>>>> 364648ff5405f479f4f2bb4f172023f566ac02b0

    [[nodiscard]] virtual std::string GetAllFilesForUser(const std::string& login) const = 0;

    virtual void InsertRootFile(const std::string& fileName, const std::string& login,
<<<<<<< HEAD
                                const std::filesystem::path& filePath) const = 0;
=======
                            const std::filesystem::path& filePath) const = 0;
>>>>>>> 364648ff5405f479f4f2bb4f172023f566ac02b0

    virtual void InsertGraphArc(const std::string& login, const std::string& rootFileName,
                                const std::string& fromFileName, const std::string& toFileName) const = 0;

    virtual void InsertNonRootFile(const std::string& fileName, const std::string& login,
                                   const std::filesystem::path& filePath) const;

    virtual void SetFileDeleted(const std::string& fileName, const std::string& login) const = 0;

protected:
    std::shared_ptr<StorageDbAdapter> m_adapter;
};
<<<<<<< HEAD
} // namespace inklink::db_controlle
=======
} // namespace inklink::db_controller
>>>>>>> 364648ff5405f479f4f2bb4f172023f566ac02b0
