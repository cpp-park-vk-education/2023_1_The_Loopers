#pragma once

#include "idb_adapter.h"

#include <memory>

namespace inklink::db_adapter
{
class StorageDbAdapter final : public DbAdapterBase
{
public:
    using DbRow = std::vector<std::string>;
    using DbTable = std::vector<DbRow>;

public:
    void Connect(const std::string& connectionString) override;

    [[nodiscard]] std::shared_ptr<pqxx::connection> GetConnection();

    void Insert(const std::string& request, const std::string&... arguments) const override;

    void Update(const std::string& request, const std::string&... arguments) const override;

    void Delete(const std::string& request) const override;

    [[nodiscard]] DbTable Select(const std::string& request, const std::string&... arguments) const override;
private:
    std::shared_ptr<pqxx::connection> m_connection;
};
} // namespace inklink::db_adapter
