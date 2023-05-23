#pragma once

#include "idb_adapter.h"

#include <pqxx/pqxx> 


namespace inklink::db_adapter
{
class StorageDbAdapter final : public DbAdapterBase
{
    ~StorageDbAdapter() = default;

    void Connect(const std::string& connectionString) override;

    [[nodiscard]] pqxx::connection& GetConnection();

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
    pqxx::connection m_connection;
};
} // namespace inklink::db_adapter
