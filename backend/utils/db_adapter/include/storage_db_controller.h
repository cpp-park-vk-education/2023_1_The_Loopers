#pragma once

#include <string>
#include "idb_adapter.h"




namespace inklink::db_controller
{
class StorageDbController
{
public:
    void SetAdapter(PgSqlIDbAdapter& adapter);

    std::string GetFilePath(std::string fileName, std::string login);
    std::string GetGraphArcs(std::string vertexName, std::string login);

    void InsertFile(std::string fileName, std::string login, std::string filePath);
    void InsertGraphArc(std::string fromFileName, std::string toFileName);

    void Connect(const std::string& connectionString);

private:
    PgSqlIDbAdapter& m_adapter;
};
} // namespace inklink::db_controller
