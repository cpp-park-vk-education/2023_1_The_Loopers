#pragma once

#include "idb_adapter.h"

#include <filesystem>
#include <string>


namespace inklink::db_controller
{
class StorageDbController final
{
public:
    explicit void SetAdapter(DbAdapterBase& adapter) override;

    void Connect(const std::string& connectionString) override;

    [[nodiscard]] std::filesystem::path GetFilePath(const std::string& fileName,
                                                    const std::string& login) const override;

    [[nodiscard]] std::string GetGraphArcs(const std::string& rootFileName, const std::string& vertexFileName,
                                           const std::string& login) const override;

    [[nodiscard]] std::string GetAllFilesForUser(const std::string& login) const override;


    void InsertFile(const std::string& fileName, const std::string& login,
                    const std::filesystem::path& filePath) const override;

    void InsertGraphArc(const std::string& rootFileName, const std::string& fromFileName, const std::string& toFileName,
                        const std::string& sessionId) const override;

    void SetFileDeleted(const std::string& fileName, const std::string& login) const override;

private:
    DbAdapterBase& m_adapter;
};
} // namespace inklink::db_controller
