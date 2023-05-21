#pragma once

#include "idb_adapter.h"

#include <pqxx/pqxx> 

#include <string>


namespace inklink::db_adapter
{
using DbTable = std::vector<std::vector<std::string>>;


class StorageDbAdapter final : public DbAdapterBase
{
    ~StorageDbAdapter() = default;

    void Connect(const std::string& connectionString) override;

    void Insert(const std::string& request) const  override;
    void Update(const std::string& request) const  override;
    void Delete(const std::string& request) const  override;
    [[nodiscard]] DbTable Select(const std::string& request) const override;

private:
    pqxx::connection m_connection;
};
} // namespace inklink::db_adapter
