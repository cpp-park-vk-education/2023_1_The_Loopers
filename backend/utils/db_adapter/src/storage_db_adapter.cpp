#include "storage_db_adapter.h"

#include <utility>

namespace inklink::db_adapter
{
void StorageDbAdapter::Connect(const std::string& connectionString)
{
    m_connection = static_cast<pqxx::connection&&>(pqxx::connection{connectionString});
}

std::shared_ptr<pqxx::connection> StorageDbAdapter::GetConnection()
{
    return std::make_shared<pqxx::connection>(m_connection);
}

template <typename... Arguments>
void StorageDbAdapter::Insert(const std::string& request, const Arguments&... arguments) const
{
    pqxx::work inserter(m_connection);
    inserter.exec_prepared(request, arguments...);

    inserter.commit();
}

template <typename... Arguments>
StorageDbAdapter::DbTable StorageDbAdapter::Select(const std::string& request, const Arguments&... arguments) const
{
    pqxx::work selector(m_connection);
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

template <typename... Arguments>
void StorageDbAdapter::Update(const std::string& request, const Arguments&... arguments) const
{
    pqxx::work updator(m_connection);
    updator.exec_prepared(request, arguments...);

    updator.commit();
}

void StorageDbAdapter::Delete(const std::string& request) const
{
    pqxx::work deletor(m_connection);
    deletor.exec(request);

    deletor.commit();
}
} // namespace inklink::db_adapter
