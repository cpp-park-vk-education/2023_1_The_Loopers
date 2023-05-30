#pragma once

#include "auth_db_adapter.h"
#include "hash_and_salt.h"

#include <string>

namespace inklink::db_controller
{
class IAuthDbController
{
public:
    using AuthDbAdapter = inklink::db_adapter::AuthDbAdapter;
    using HashAndSalt = inklink::auth_handler::HashAndSalt;

public:
    virtual void SetAdapter(const std::shared_ptr<AuthDbAdapter>& adapter) = 0;

    virtual void Run(const std::string& connectionString) = 0;

    virtual bool InsertUser(const std::string& login, const std::string& password, const std::string& salt) const = 0;

    [[nodiscard]] virtual HashAndSalt GetPasswordAndSalt(const std::string& login) const = 0;

    [[nodiscard]] virtual std::string FindUser(const std::string& login) const = 0;

protected:
    std::shared_ptr<AuthDbAdapter> m_adapter;
};
} // namespace inklink::db_controller
