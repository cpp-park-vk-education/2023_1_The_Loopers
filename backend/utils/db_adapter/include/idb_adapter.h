#pragma once

#include <pqxx/pqxx> 

#include <memory>
#include <string>
#include <vector>


namespace inklink::db_adapter
{
template <typename Implementation, typename Connection>
class DbAdapterBase
{
public:
    using DbRow = std::vector<std::string>;
    using DbTable = std::vector<DbRow>;

public:
    void Connect(const std::string& connectionString);
    [[nodiscard]] std::shared_ptr<Connection> GetConnection();

    template <typename... Arguments>
    void Insert(const std::string& request, const Arguments&... arguments) const;

    template <typename... Arguments>
    void Update(const std::string& request, const Arguments&... arguments) const;

    template <typename... Arguments>
    [[nodiscard]] DbTable Select(const std::string& request, const Arguments&... arguments) const;

    void Delete(const std::string& request) const;

protected:
    std::unique_ptr<Connection> m_connection;
};
} // namespace inklink::db_adapter
