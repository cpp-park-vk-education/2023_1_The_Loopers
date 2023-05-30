#pragma once

#include "idb_adapter.h"

#include <memory>

namespace inklink::db_adapter
{
class StorageDbAdapter final : DbAdapterBase<StorageDbAdapter, pqxx::connection>
{
public:
    using DbRow = std::vector<std::string>;
    using DbTable = std::vector<DbRow>;

public:
    void Connect(const std::string& connectionString)
    {
        m_connection = std::make_shared<pqxx::connection>(connectionString);
    }

    [[nodiscard]] std::shared_ptr<pqxx::connection> GetConnection()
    {
        return m_connection;
    }

    template <typename... Arguments>
    void Insert(const std::string& request, const Arguments&... arguments) const
    {
        pqxx::work inserter(*m_connection);
        inserter.exec_prepared(request, arguments...);

        inserter.commit();
    }

    template <typename... Arguments>
    void Update(const std::string& request, const Arguments&... arguments) const
    {
        pqxx::work updator(*m_connection);
        updator.exec_prepared(request, arguments...);

        updator.commit();
    }

    template <typename... Arguments>
    [[nodiscard]] DbTable Select(const std::string& request, const Arguments&... arguments) const
    {
        pqxx::work selector(*m_connection);
        DbTable result{};

        pqxx::result response = selector.exec_prepared(request, arguments...);

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

    void Delete(const std::string& request) const
    {
        pqxx::work deletor(*m_connection);
        deletor.exec(request);

        deletor.commit();
    }
};
} // namespace inklink::db_adapter
