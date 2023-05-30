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
    void Connect(const std::string& connectionString)
    {
        static_cast<Implementation*>(this)->Connect(connectionString);
    }

    [[nodiscard]] std::shared_ptr<Connection> GetConnection()
    {
        static_cast<Implementation*>(this)->GetConnection();
    }

    template <typename... Arguments>
    void Insert(const std::string& request, const Arguments&... arguments) const
    {
        static_cast<Implementation*>(this)->Insert(request, arguments...);
    }

    template <typename... Arguments>
    void Update(const std::string& request, const Arguments&... arguments) const
    {
        static_cast<Implementation*>(this)->Update(request, arguments...);
    }

    template <typename... Arguments>
    [[nodiscard]] DbTable Select(const std::string& request, const Arguments&... arguments) const
    {
        static_cast<Implementation*>(this)->Select(request, arguments...);
    }

    void Delete(const std::string& request) const
    {
        static_cast<Implementation*>(this)->Delete(request);
    }

protected:
    std::shared_ptr<Connection> m_connection;
};
} // namespace inklink::db_adapter