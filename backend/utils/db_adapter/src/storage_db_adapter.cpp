#include "storage_db_adapter.h"
#include "table_after_select"

#include <pqxx/pqxx> 

#include <string>
#include <vector>


namespace inklink::db_adapter
{
void StorageDbAdapter::Connect(const std::string& connectionString) 
{
    m_connection(connectionString.c_str());
    return m_connection.is_open();
}

void StorageDbAdapter::Insert(const std::string& request)
{
    pqxx::work inserter(m_connection);
    inserter.exec(request);

    inserter.commit();
}

TableAfterSelect StorageDbAdapter::Select(const std::string& request)
{
    pqxx::work selector(m_connection);
    TableAfterSelect result;

    pqxx::result response = selector.exec(request);

    for (auto row: response)
    {
        std::vector<std::string> currentRow;

        for (auto field : row)
        {
            currentRow.push_back(field);
        }

        result.PushBackRow(currentRow);
    }

    return result;
}

void StorageDbAdapter::Update(const std::string& request)
{
    pqxx::work updator(m_connection);
    updator.exec(request);

    updator.commit();
}

void StorageDbAdapter::Delete(const std::string& request)
{
    pqxx::work deletor(m_connection);
    deletor.exec(request);

    deletor.commit();
}
} // namespace inklink::db_adapter
