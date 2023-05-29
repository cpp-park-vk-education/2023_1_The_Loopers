#pragma once

#include <pqxx/pqxx> 

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

    template <typename... Arguments>
    void Insert(const std::string& request, const Arguments&... arguments) const;

    template <typename... Arguments>
    void Update(const std::string& request, const Arguments&... arguments) const;

    template <typename... Arguments>
    [[nodiscard]] DbTable Select(const std::string& request, const Arguments&... arguments) const;

    void Delete(const std::string& request) const;

protected:
    Connection m_connection;
};
} // namespace inklink::db_adapter
