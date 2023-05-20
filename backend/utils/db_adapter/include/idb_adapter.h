#pragma once

#include "table_after_select.h"

#include <pqxx/pqxx>

#include <string>


namespace inklink::db_adapter
{
using namespace data_types;


class DbAdapterBase
{
public:
    virtual ~DbAdapterBase() = default;

    virtual void Connect(const std::string& connectionString) = 0;
    virtual void Insert(const std::string& request) const = 0;
    virtual void Update(const std::string& request) const = 0;
    virtual void Delete(const std::string& request) const = 0;
    [[nodiscard]] virtual TableAfterSelect Select(const std::string& request) const = 0;

protected:
    pqxx::connection m_connection;
};
} // namespace inklink::db_adapter
