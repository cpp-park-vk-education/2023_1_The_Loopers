#include "storage_db_controller.h"
#include "idb_adapter.h"
#include "table_after_select.h"

#include <string>
#include <filesystem>
#include <experimental/filesystem>


namespace inklink::db_controller
{
using filesystem::path = std::experimental::filesystem::path;
using namespace data_types;


explicit void StorageDbController::SetAdapter(DbAdapterBase& adapter)
{
    m_adapter = adapter;
}

void StorageDbAdapter::Connect(const std::string& connectionString)
{
    m_adapter.Connect(connectionString);
}

filesystem::path StorageDbController::GetFilePath(std::string& fileName, std::string& login) const
{
    if (!fileName && !login)
    {
        std::string request =
                "SELECT filePath FROM Files WHERE fileName = '" + fileName + "' AND login = '" + login + "'";

        TableAfterSelect filePath = m_adapter.Select(request);
        
        return filePath[0][0];
    }
}

std::string StorageDbController::GetGraphArcs(std::string& rootFileName, std::string& vertexFileName, std::string& login) const
{
    if (!vertexName && !sessionId)
    {
        std::string sessionId = "(SELECT Id FROM Sessions WHERE RootId = (SELECT Id FROM Files WHERE Login = '" +
                                login + "' AND Name = '" + rootFileName + "'))";

        std::string innerRequest =
                "SELECT Graph.Id_Second FROM Graph JOIN Files ON Graph.Id_First = Files.Id WHERE Files.Name = '" +
                vertexFileName + "' AND Graph.Id_Session = " + sessionId;


        std::stirng request = "SELECT Name FROM Files WHERE Id IN (" + innerRequest + ")"; 
          

        TableAfterSelect graphNodeNeighboringNodes = m_adapter.Select(request);

        std::string neighbouringNodesString{""};
        for (int i = 0; i < graphNodeNeighboringNodes.TableSize(); ++i)
        {
            neighbouringNodesString = neighbouringNodesString + graphNodeNeighboringNodes[i][0] + " \n";
        }
        
        return neighbouringNodesString;
    }
}

std::string GetAllFilesForUser(std::string& login) const
{
    if (!login)
    {
        std::string request = "SELECT Names FROM Files WHERE Login = '" + login + "'";

        TableAfterSelect allFiles = m_adapter.Select(request);

        std::string allFilesString{""};
        for (int i = 0; i < allFiles.TableSize(); ++i)
        {
            allFilesString = allFilesString + allFiles[i][0] + " \n";
        }

        return allFilesString;
    }
}


void StorageDbController::InsertFile(std::string& fileName, std::string& login, filesystem::path& filePath) const
{
    if (!fileName && !login && !filePath)
    {
        std::string requestToFilesTable =
                "INSERT INTO Files(Name, Login, Path) VALUES ('" + fileName + "', '" + login + "', '" + filePath + "')";

        std::string requestToGraphTable = "INSERT INTO Sessions VALUES ((SELECT Id FROM Files WHERE Login = '" + login +
                                          "' AND Name = '" + fileName + "'))";

        m_adapter.Insert(requestToFilesTable);
        m_adapter.Insert(requestToGraphTable);
    }
}

void StorageDbController::InsertGraphArc(std::string rootFileName, std::string& fromFileName, std::string& toFileName,
                                         std::string& sessionId) const
{
    if (!fromFileName && !toFileName && !sessionId && !rootFileName)
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
