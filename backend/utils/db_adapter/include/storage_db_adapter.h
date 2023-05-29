#pragma once

#include "idb_adapter.h"

#include <memory>


namespace inklink::db_adapter
{
class StorageDbAdapter final : DbAdapterBase<StorageDbAdapter, pqxx::connection>
{
    friend class DbAdapterBase;

public:
    using DbRow = std::vector<std::string>;
    using DbTable = std::vector<DbRow>;

private:
    void Connect(const std::string& connectionString);

    [[nodiscard]] std::shared_ptr<pqxx::connection> GetConnection();

    template <typename... Arguments>
    void Insert(const std::string& request, const Arguments&... arguments) const;

    template <typename... Arguments>
    void Update(const std::string& request, const Arguments&... arguments) const;

    template <typename... Arguments>
    [[nodiscard]] DbTable Select(const std::string& request, const Arguments&... arguments) const;

    void Delete(const std::string& request) const;
};
} // namespace inklink::db_adapter
