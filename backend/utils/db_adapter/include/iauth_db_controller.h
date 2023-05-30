#pragma once

#include "idb_adapter.h"

#include <string>

namespace inklink::db_controller
{
class IAuthDbController
{
public:
    using HashAndSalt = inklink::auth_handler::HashAndSalt;

public:
    virtual void SetAdapter(const DbAdapterBase& adapter) = 0;

    virtual void Run(const std::string& connectionString) = 0;

    virtual bool InsertUser(const std::string& login, const std::string& password) const = 0;

    [[nodiscard]] virtual HashAndSalt GetPasswordAndSalt(const std::string& login) const = 0;

    [[nodiscard]] virtual std::string FindUser(const std::string& login) const = 0;

protected:
    DbAdapterBase& m_adapter;
};
} // namespace inklink::db_controller
