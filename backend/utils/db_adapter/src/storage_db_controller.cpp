#include "storage_db_controller.h"
#include "idb_adapter.h"

#include <string>
#include <filesystem>
#include <experimental/filesystem>


namespace inklink::db_controller
{
namespace filesystem = std::experimental::filesystem;


explicit void StorageDbController::SetAdapter(DbAdapterBase& adapter)
{
    if (adapter)
    {
        m_adapter = adapter;
    }
}

filesystem::path StorageDbController::GetFilePath(std::string fileName, std::string login)
{
    if (!fileName && !login)
    {
        std::string request =
                "SELECT filePath FROM Files WHERE fileName = '" + fileName + "' AND login = '" + login + "'";
        filesystem::path filePath = m_adapter.Select(request);
        
        return filePath;
    }
}

std::string StorageDbController::GetGraphArcs(std::string vertexName, std::string login)
{
    if (!vertexName && !login)
    {
        std::string innerRequest =
                "SELECT Graph.Id_Second FROM Graph Join Files ON Graph.Id_First = Files.Id WHERE Files.Login = '" +
                login + "' AND Files.Name = '" + vertexName + "'";

        std::stirng request = "SELECT Name FROM Files WHERE Id IN (" + innerRequest + ")"; 
                            
        return m_adapter.Select(request);
    }
}

void StorageDbController::InsertFile(std::string fileName, std::string login, filesystem::path filePath)
{
    if (!fileName && !login && !filePath)
    {
        std::string request =
                "INSERT INTO Files(Name, Login, Path) VALUES ('" + fileName + "', '" + login + "', '" + filePath + "')";

        m_adapter.Insert(request);
    }
}

void StorageDbController::InsertGraphArc(std::string fromFileName, std::string toFileName, std::string login)
{
    if (!fromFileName && !toFileName && !login)
    {
        std::string idFirst =
                "(SELECT Id FROM Files LEFT JOIN Graph ON Files.Id = Graph.Id_First WHERE Files.Login = '" + login +
                "' AND Name = '" + fromFileName + "')";

        std::string idSecond =
                "(SELECT Id FROM Files LEFT JOIN Graph ON Files.Id = Graph.Id_First WHERE Files.Login = '" + login +
                "' AND Name = '" + toFileName + "')";

        std::string request = "INSERT INTO Graph VALUES (" + idFirst + ", " + idSecond + ", '" + login + "')";

        m_adapter.Insert(request);
    }
}
} // namespace inklink::db_controller
