#include "storage_db_adapter.h"

#include <pqxx/pqxx> 

#include <string>


namespace inklink::db_adapter
{
void StorageDbAdapter::Connect(const std::string& connectionString)
{
    m_connection = std::move(pqxx::connection(connectionString));
}

void StorageDbAdapter::Insert(const std::string& request) const
{
    pqxx::work inserter(m_connection);
    inserter.exec(request);

    inserter.commit();
}

DbTable StorageDbAdapter::Select(const std::string& request) const
{
    pqxx::work selector(m_connection);
    DbTable result;

    pqxx::result response = selector.exec(request);

    for (const auto& row: response)
    {
        DbRow currentRow;

        for (const auto& field : row)
        {
            currentRow.push_back(field.as<std::string>());
        }

        result.push_back(currentRow);
    }

    return result;
}

void StorageDbAdapter::Update(const std::string& request) const
{
    pqxx::work updator(m_connection);
    updator.exec(request);

    updator.commit();
}

void StorageDbAdapter::Delete(const std::string& request) const
{
    pqxx::work deletor(m_connection);
    deletor.exec(request);

    deletor.commit();
}
} // namespace inklink::db_adapter
