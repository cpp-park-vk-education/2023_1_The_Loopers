#pragma once

#include "iauth_db_controller.h"
#include "hash_and_salt.h"

#include <string>


namespace inklink::db_controller
{
class AuthDbController : public IAuthDbController
{
public:
    using DbRow = std::vector<std::string>;
    using DbTable = std::vector<DbRow>;
    using HashAndSalt = inklink::auth_handler::HashAndSalt;

public:
    void SetAdapter(const std::shared_ptr<AuthDbAdapter>& adapter) override;

    void Run(const std::string& connectionString) override;

    bool InsertUser(const std::string& login, const std::string& password, const std::string& salt) const override;

    [[nodiscard]] HashAndSalt GetPasswordAndSalt(const std::string& login) const override;

    [[nodiscard]] std::string FindUser(const std::string& login) const override;
};
} // namespace inklink::db_controller
