#include "storage_db_controller.h"


namespace inklink::db_controller
{
void StorageDbController::SetAdapter(DbAdapterBase& adapter)
{
    m_adapter = adapter;
}

void StorageDbController::Run(const std::string& connectionString)
{
    m_adapter.Connect(connectionString);

    pqxx::connection settings = m_adapter.GetConnection();

    settings.prepare("GetFilePath",
                     "SELECT filePath FROM Files "
                     "WHERE fileName = $1 AND login = $2 AND Deleted_at IS NULL");

    settings.prepare("GetGraphArcs",
                     "SELECT Name FROM Files "
                     "WHERE Id IN "
                     "(SELECT Graph.Id_Second FROM Graph JOIN Files "
                     "ON Graph.Id_First = Files.Id "
                     "WHERE Files.Name = $1 AND Graph.Id_Session = "
                     "(SELECT Id FROM Sessions "
                     "WHERE RootId = (SELECT Id FROM Files WHERE Login = $2 AND Name = $3))) AND Deleted_at IS NULL");

    settings.prepare("GetAllFilesForUser", "SELECT Names FROM Files WHERE Login = $1 AND Deleted_at IS NULL");

    settings.prepare("InsertFile", "INSERT INTO Files(Name, Login, Path) VALUES ($1, $2, $3)");

    settings.prepare("InsertNewSession",
                     "INSERT INTO Sessions VALUES ((SELECT Id FROM Files WHERE Login = $1 AND Name = $2))");

    settings.prepare("InsertGraphArc",
                     "INSERT INTO Graph VALUES "
                     "((SELECT Id FROM Files WHERE Login = $1 AND Name = $2), "
                     "(SELECT Id FROM Files WHERE Login = $1 AND Name = $3), "
                     "(SELECT Id FROM Sessions WHERE RootId = "
                     "(SELECT Id FROM Files WHERE Login = $1 AND Name = $4)))");

    settings.prepare("SetFileDeleted",
                     "UPDATE Files SET Deleted_at = CURRENT_TIMESTAMP WHERE Login = $1 AND Name = $2");
}

std::filesystem::path StorageDbController::GetFilePath(const std::string& fileName, const std::string& login) const
{
    if (!(fileName.empty() || login.empty()))
    {
        DbTable filePath = m_adapter.Select("GetFilePath", fileName, login);
        
        return filePath[0][0];
    }
}

std::string StorageDbController::GetGraphArcs(const std::string& rootFileName, const std::string& vertexFileName,
                                              const std::string& login) const
{
    if (!(vertexName.empty() || sessionId.empty()))
    {
        DbTable graphNodeNeighboringNodes = m_adapter.Select("GetGraphArcs", vertexFileName, login, rootFileName);

        std::string neighbouringNodesString{};
        for (int i = 0; i < graphNodeNeighboringNodes.TableSize(); ++i)
        {
            neighbouringNodesString = neighbouringNodesString + graphNodeNeighboringNodes[i][0] + " \n";
        }
        
        return neighbouringNodesString;
    }
}

std::string StorageDbController::GetAllFilesForUser(const std::string& login) const
{
    if (!login.empty())
    {
        DbTable allFiles = m_adapter.Select("GetAllFilesForUser", login);

        std::string allFilesString{};
        for (int i = 0; i < allFiles.TableSize(); ++i)
        {
            allFilesString = allFilesString + allFiles[i][0] + " \n";
        }

        return allFilesString;
    }
}


void StorageDbController::InsertRootFile(const std::string& fileName, const std::string& login,
                                     const std::filesystem::path& filePath) const
{
    if (!(fileName.empty() || login.empty() || !filePath.empty()))
    {
        m_adapter.Insert("InsertFile", fileName, login, filePath);
        m_adapter.Insert("InsertNewSession", login, fileName);
    }
}

void StorageDbController::InsertNonRootFile(const std::string& rootFileName, const std::string& fileName,
                                            const std::string& login, const std::filesystem::path& filePath) const
{
    if (!(fileName.empty() || rootFileName.empty() || filePath.empty() || login.empty()))
    {
        m_adapter.Insert("InsertFile", fileName, login, filePath);
    }
}

void StorageDbController::InsertGraphArc(const std::string& rootFileName, const std::string& fromFileName,
                                         const std::string& toFileName) const
{
    if (!(fromFileName.empty() || toFileName.empty() || rootFileName.empty()))
    {
        m_adapter.Insert("InsertGraphArc", login, fromFileName, toFileName, rootFileName);
    }
}

void StorageDbController::SetFileDeleted(const std::string& fileName, const std::string& login) const
{
    m_adapter.Update("SetFileDeleted", login, fileName);
}
} // namespace inklink::db_controller
