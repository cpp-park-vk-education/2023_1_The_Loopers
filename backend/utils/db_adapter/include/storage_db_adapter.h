#pragma once

#include "idb_adapter.h"

#include <pqxx/pqxx> 


namespace inklink::db_adapter
{
class StorageDbAdapter final : public DbAdapterBase
{
    ~StorageDbAdapter() = default;

    void Connect(const std::string& connectionString) override;
    [[nodiscard]] pqxx::connection& GetConnection() const;

    void Insert(const std::string& request) const  override;
    void Update(const std::string& request) const  override;
    void Delete(const std::string& request) const  override;
    [[nodiscard]] DbTable Select(const std::string& request) const override;

private:
    pqxx::connection m_connection;
};
} // namespace inklink::db_adapter
