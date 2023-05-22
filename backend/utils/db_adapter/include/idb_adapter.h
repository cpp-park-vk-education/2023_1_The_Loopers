#pragma once

#include <pqxx/pqxx>

#include <string>
#include <vector>


namespace inklink::db_adapter
{
using DbRow = std::vector<std::string>;
using DbTable = std::vector<DbRow>;


class DbAdapterBase
{
public:
    virtual ~DbAdapterBase() = default;

    virtual void Connect(const std::string& connectionString) = 0;
    virtual void Insert(const std::string& request) const = 0;
    virtual void Update(const std::string& request) const = 0;
    virtual void Delete(const std::string& request) const = 0;
    [[nodiscard]] virtual DbTable Select(const std::string& request) const = 0;
};
} // namespace inklink::db_adapter
