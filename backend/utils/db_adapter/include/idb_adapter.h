#pragma once

#include <string>
#include <pqxx/pqxx>

namespace inklink::db_adapter
{
class IDBAdapter
{
public:
    virtual ~IDBAdapter() = default;

    virtual void Insert(const std::string& insertPart, const std::string& valuesPart) = 0;

    virtual void Update(const std::string& updatePart, const std::string& setPart, const std::stirng wherePart) = 0;

    virtual std::string Select(const std::string& selectPart, const std::string& fromPart, const std::string& wherePart,
                               const std::string& orderPart) = 0;

    virtual void Delete(const std::string& deletePart, const std::string& fromPart, const std::string& wherePart) = 0;

    virtual void Connect(const std::string& connectionString) = 0;

protected:
    pqxx::connection m_connection;
};
} // namespace inklink::db_adapter
