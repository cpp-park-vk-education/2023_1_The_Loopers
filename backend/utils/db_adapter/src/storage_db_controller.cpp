#include "storage_db_controller.h"
#include "idb_adapter.h"


namespace inklink::db_controller
{
explicit void StorageDbController::SetAdapter(DbAdapterBase& adapter)
{
    m_adapter = adapter;
}

void StorageDbAdapter::Connect(const std::string& connectionString)
{
    m_adapter.Connect(connectionString);
}

std::filesystem::path StorageDbController::GetFilePath(const std::string& fileName, const std::string& login) const
{
    if (!(fileName || login))
    {
        std::string request =
                "SELECT filePath FROM Files WHERE fileName = '" + fileName + "' AND login = '" + login + "'";

        DbTable filePath = m_adapter.Select(request);
        
        return filePath[0][0];
    }
}

std::string StorageDbController::GetGraphArcs(const std::string& rootFileName, const std::string& vertexFileName,
                                              const std::string& login) const
{
    if (!(vertexName || sessionId))
    {
        std::string sessionId = "(SELECT Id FROM Sessions WHERE RootId = (SELECT Id FROM Files WHERE Login = '" +
                                login + "' AND Name = '" + rootFileName + "'))";

        std::string innerRequest =
                "SELECT Graph.Id_Second FROM Graph JOIN Files ON Graph.Id_First = Files.Id WHERE Files.Name = '" +
                vertexFileName + "' AND Graph.Id_Session = " + sessionId;


        std::stirng request = "SELECT Name FROM Files WHERE Id IN (" + innerRequest + ")"; 
          

        DbTable graphNodeNeighboringNodes = m_adapter.Select(request);

        std::string neighbouringNodesString{""};
        for (int i = 0; i < graphNodeNeighboringNodes.TableSize(); ++i)
        {
            neighbouringNodesString = neighbouringNodesString + graphNodeNeighboringNodes[i][0] + " \n";
        }
        
        return neighbouringNodesString;
    }
}

std::string GetAllFilesForUser(const std::string& login) const
{
    if (!login)
    {
        std::string request = "SELECT Names FROM Files WHERE Login = '" + login + "'";

        DbTable allFiles = m_adapter.Select(request);

        std::string allFilesString{""};
        for (int i = 0; i < allFiles.TableSize(); ++i)
        {
            allFilesString = allFilesString + allFiles[i][0] + " \n";
        }

        return allFilesString;
    }
}


void StorageDbController::InsertFile(const std::string& fileName, const std::string& login,
                                     const std::filesystem::path& filePath) const
{
    if (!(fileName || login || !filePath))
    {
        std::string requestToFilesTable =
                "INSERT INTO Files(Name, Login, Path) VALUES ('" + fileName + "', '" + login + "', '" + filePath + "')";

        std::string requestToGraphTable = "INSERT INTO Sessions VALUES ((SELECT Id FROM Files WHERE Login = '" + login +
                                          "' AND Name = '" + fileName + "'))";

        m_adapter.Insert(requestToFilesTable);
        m_adapter.Insert(requestToGraphTable);
    }
}

void StorageDbController::InsertGraphArc(const std::string rootFileName, const std::string& fromFileName,
                                         const std::string& toFileName, const std::string& sessionId) const
{
    if (!(fromFileName || toFileName || sessionId || rootFileName))
    {
        std::string idFirst = "(SELECT Id FROM Files WHERE Login = '" + login "' AND Name = '" + fromFileName + "')";

        std::string idSecond = "(SELECT Id FROM Files WHERE Login = '" + login "' AND Name = '" + toFileName + "')";

        std::string sessionId = "(SELECT Id FROM Sessions WHERE RootId = (SELECT Id FROM Files WHERE Login = '" +
                                login + "' AND Name = '" + rootFileName + "'))";

        std::string request = "INSERT INTO Graph VALUES (" + idFirst + ", " + idSecond + ", " + sessionId + ")";

        m_adapter.Insert(request);
    }
}
} // namespace inklink::db_controller
