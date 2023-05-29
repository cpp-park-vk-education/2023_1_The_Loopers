#include "storage_db_controller.h"

#include <memory>


namespace inklink::db_controller
{
void StorageDbController::SetAdapter(DbAdapterBase& adapter)
{
    m_adapter = adapter;
}

void StorageDbController::Run(const std::string& connectionString)
{
    m_adapter.Connect(connectionString);

    std::shared_ptr<pqxx::connection> settings = m_adapter.GetConnection();

    settings->prepare("GetFilePath",
                     "SELECT path FROM Files "
                     "WHERE name = $1 AND login = $2 AND deleted_at IS NULL");

    settings->prepare("GetGraphArcs",
                     "SELECT name FROM Files "
                     "WHERE id IN "
                     "(SELECT Graph.id_second FROM Graph JOIN Files "
                     "ON Graph.id_first = Files.id "
                     "WHERE Files.name = $1 AND Graph.id_session = "
                     "(SELECT id FROM Sessions "
                     "WHERE root_id = (SELECT id FROM Files WHERE login = $2 AND name = $3))) AND deleted_at IS NULL");

    settings->prepare("GetAllFilesForUser", "SELECT name FROM Files WHERE login = $1 AND deleted_at IS NULL");

    settings->prepare("InsertFile", "INSERT INTO Files(name, login, path) VALUES ($1, $2, $3)");

    settings->prepare("InsertNewSession",
                     "INSERT INTO Sessions VALUES ((SELECT id FROM Files WHERE login = $1 AND name = $2))");

    settings->prepare("InsertGraphArc",
                     "INSERT INTO Graph VALUES "
                     "((SELECT id FROM Files WHERE login = $1 AND name = $2), "
                     "(SELECT id FROM Files WHERE login = $1 AND name = $3), "
                     "(SELECT id FROM Sessions WHERE root_id = "
                     "(SELECT id FROM Files WHERE login = $1 AND name = $4)))");

    settings->prepare("SetFileDeleted",
                     "UPDATE Files SET deleted_at = CURRENT_TIMESTAMP WHERE login = $1 AND name = $2");
}

std::filesystem::path StorageDbController::GetFilePath(const std::string& fileName, const std::string& login) const
{
    if (fileName.empty() || login.empty())
    {
        return "";
    }

    DbTable filePath = m_adapter.Select("GetFilePath", fileName, login);

    return filePath[0][0];
}

std::string StorageDbController::GetGraphArcs(const std::string& rootFileName, const std::string& vertexFileName,
                                              const std::string& login) const
{
    if (vertexName.empty() || rootFileName.empty() || login.empty())
    {
        return "";
    }

    DbTable graphNodeNeighboringNodes = m_adapter.Select("GetGraphArcs", vertexFileName, login, rootFileName);

    std::string neighbouringNodesString{};
    for (const auto& row : graphNodeNeighboringNodes)
    {
        neighbouringNodesString = neighbouringNodesString + row[0] + " \n";
    }

    return neighbouringNodesString;
}

std::string StorageDbController::GetAllFilesForUser(const std::string& login) const
{
    if (login.empty())
    {
        return "";
    }

    DbTable allFiles = m_adapter.Select("GetAllFilesForUser", login);

    std::string allFilesString{};
    for (const auto& row : allFiles)
    {
        allFilesString = allFilesString + row[0] + " \n";
    }

    return allFilesString;
}


void StorageDbController::InsertRootFile(const std::string& fileName, const std::string& login,
                                     const std::filesystem::path& filePath) const
{
    if (fileName.empty() || login.empty() || !filePath.empty())
    {
        return;
    }

    m_adapter.Insert("InsertFile", fileName, login, filePath);
    m_adapter.Insert("InsertNewSession", login, fileName);
}

void StorageDbController::InsertNonRootFile(const std::string& rootFileName, const std::string& fileName,
                                            const std::string& login, const std::filesystem::path& filePath) const
{
    if (fileName.empty() || rootFileName.empty() || filePath.empty() || login.empty())
    {
        return;
    }

    m_adapter.Insert("InsertFile", fileName, login, filePath);
}

void StorageDbController::InsertGraphArc(const std::string& rootFileName, const std::string& fromFileName,
                                         const std::string& toFileName) const
{
    if (fromFileName.empty() || toFileName.empty() || rootFileName.empty())
    {
        return;
    }

    m_adapter.Insert("InsertGraphArc", login, fromFileName, toFileName, rootFileName);
}

void StorageDbController::SetFileDeleted(const std::string& fileName, const std::string& login) const
{
    m_adapter.Update("SetFileDeleted", login, fileName);
}
} // namespace inklink::db_controller
