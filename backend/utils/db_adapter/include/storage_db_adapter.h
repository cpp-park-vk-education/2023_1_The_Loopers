#pragma once

#include "idb_adapter.h"

#include <pqxx/pqxx> 

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

    void Insert(const std::string& request, const std::string& argument1, const std::string& argument2,
                const std::string& argument3, const std::string& argument4) const override;

    void Insert(const std::string& request, const std::string& argument1, const std::string& argument2,
                const std::string& argument3) const override;

    void Update(const std::string& request, const std::string& argument1, const std::string& argument2) const override;

    void Delete(const std::string& request) const override;

    [[nodiscard]] DbTable Select(const std::string& request, const std::string& argument) const override;

    [[nodiscard]] DbTable Select(const std::string& request, const std::string& argument1,
                                 const std::string& argument2) const override;

    [[nodiscard]] DbTable Select(const std::string& request, const std::string& argument1, const std::string& argument2,
                                 const std::string& argument3) const override;

private:
    std::shared_ptr<pqxx::connection> m_connection;
};
} // namespace inklink::db_adapter
