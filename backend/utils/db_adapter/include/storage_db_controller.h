#pragma once

#include "istorage_db_controller.h"


namespace inklink::db_controller
{
class StorageDbController final : public IStorageDbController
{
public:
    using DbRow = std::vector<std::string>;
    using DbTable = std::vector<DbRow>;

    void SetAdapter(DbAdapterBase& adapter) override;

    void Run(const std::string& connectionString) override;

    [[nodiscard]] std::filesystem::path GetFilePath(const std::string& fileName,
                                                    const std::string& login) const override;

    [[nodiscard]] std::string GetGraphArcs(const std::string& rootFileName, const std::string& vertexFileName,
                                           const std::string& login) const override;

    [[nodiscard]] std::string GetAllFilesForUser(const std::string& login) const override;


    void InsertRootFile(const std::string& fileName, const std::string& login,
                    const std::filesystem::path& filePath) const override;

    void InsertNonRootFile(const std::string& rootFileName, const std::string& fileName, const std::string& login,
                           const std::filesystem::path& filePath) const override;

    void InsertGraphArc(const std::string& rootFileName, const std::string& fromFileName, const std::string& toFileName) const override;

    void SetFileDeleted(const std::string& fileName, const std::string& login) const override;

private:
    DbAdapterBase m_adapter;
};
} // namespace inklink::db_controller
