#include "storage_db_adapter.h"


namespace inklink::db_adapter
{
void StorageDbAdapter::Connect(const std::string& connectionString)
{
    m_connection = std::make_shared<pqxx::connection>(connectionString);
}

std::shared_ptr<pqxx::connection> StorageDbAdapter::GetConnection()
{
    return m_connection;
}

void StorageDbAdapter::Insert(const std::string& request, const std::string&... arguments) const
{
    pqxx::work inserter(*m_connection);
    inserter.exec_prepared(request, arguments...);

    inserter.commit();
}

StorageDbAdapter::DbTable StorageDbAdapter::Select(const std::string& request, const std::string&... arguments) const
{
    pqxx::work selector(*m_connection);
    DbTable result{};

    pqxx::result response = selector.exec_prepared(request, arguments...);

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

void StorageDbAdapter::Update(const std::string& request, const std::string&... arguments) const
{
    pqxx::work updator(*m_connection);
    updator.exec_prepared(request, arguments...);

    updator.commit();
}

void StorageDbAdapter::Delete(const std::string& request) const
{
    pqxx::work deletor(*m_connection);
    deletor.exec(request);

    deletor.commit();
}
} // namespace inklink::db_adapter
