#pragma once

#include "idb_adapter.h"

#include <string>

namespace inklink::db_controller
{
class IAuthDbController
{
public:
    virtual void SetAdapter(DbAdapterBase& adapter) = 0;

    virtual void Run(const std::string& connectionString) = 0;

    virtual void InsertUser(const std::string& login, const std::string& password) = 0;

    virtual std::string GetPassword(const std::string& login) = 0;

protected:
    DbAdapterBase& m_adapter;
};
} // namespace inklink::db_controller
