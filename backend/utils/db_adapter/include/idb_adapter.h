#pragma once

#include <pqxx/pqxx>

#include <string>


namespace inklink::db_adapter
{
class DbAdapterBase
{
public:
    virtual ~DbAdapterBase() = default;

    virtual void Insert(const std::string& request) = 0;
    virtual void Update(const std::string& request) = 0;
    virtual void Delete(const std::string& request) = 0;
    virtual void Connect(const std::string& connectionString) = 0;
    virtual std::string Get(const std::string& request) = 0;

protected:
    pqxx::connection m_connection;
};
} // namespace inklink::db_adapter
