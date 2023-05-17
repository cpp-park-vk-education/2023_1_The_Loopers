#pragma once

#include <string>
#include "idb_adapter.h"




namespace inklink::db_controller
{
class StorageDbController
{
public:
    void SetAdapter(PgSqlIDbAdapter& adapter);

    void Insert();
    std::string Select(const std::string& table, const std::string& columnToSelect, );
    void Update();
    void Delete();

    void Connect(const std::string& connectionString);

private:
    PgSqlIDbAdapter& m_adapter;
};
} // namespace inklink::db_controller
