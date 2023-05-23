#include "storage_db_adapter.h"


namespace inklink::db_adapter
{
void StorageDbAdapter::Connect(const std::string& connectionString)
{
    m_connection = pqxx::connection(connectionString);
}

pqxx::connection& GetConnection()
{
    return m_connection;
}

void StorageDbAdapter::Insert(const std::string& request, const std::string& argument1, const std::string& argument2,
                              const std::string& argument3, const std::string& argument4) const
{
    pqxx::work inserter(m_connection);
    inserter.exec_prepared(request, argument1, argument2, argument3, argument4);

    inserter.commit();
}

void StorageDbAdapter::Insert(const std::string& request, const std::string& argument1, const std::string& argument2,
                              const std::string& argument3) const
{
    pqxx::work inserter(m_connection);
    inserter.exec_prepared(request, argument1, argument2, argument3);

    inserter.commit();
}

DbTable StorageDbAdapter::Select(const std::string& request, const std::string& argument) const
{
    pqxx::work selector(m_connection);
    DbTable result;

    pqxx::result response = selector.exec_prepared(request, argument);

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

DbTable StorageDbAdapter::Select(const std::string& request, const std::string& argument1,
                                 const std::string& argument2) const
{
    pqxx::work selector(m_connection);
    DbTable result;

    pqxx::result response = selector.exec_prepared(request, argument1, argument2);

    for (const auto& row : response)
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

DbTable StorageDbAdapter::Select(const std::string& request, const std::string& argument1,
                                 const std::string& argument2, const std::string& argument3) const
{
    pqxx::work selector(m_connection);
    DbTable result;

    pqxx::result response = selector.exec_prepared(request, argument1, argument2, argument3);

    for (const auto& row : response)
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

void StorageDbAdapter::Update(const std::string& request, const std::string& argument1,
                              const std::string& argument2) const
{
    pqxx::work updator(m_connection);
    updator.exec_prepared(request, argument1, argument2);

    updator.commit();
}

void StorageDbAdapter::Delete(const std::string& request) const
{
    pqxx::work deletor(m_connection);
    deletor.exec(request);

    deletor.commit();
}
} // namespace inklink::db_adapter
