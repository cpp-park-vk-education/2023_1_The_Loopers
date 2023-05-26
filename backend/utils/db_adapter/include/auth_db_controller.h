#pragma once

#include "iauth_db_controller.h"

#include <string>


namespace inklink::db_controller
{
class AuthDbController : public IAuthDbController
{
public:
    using DbRow = std::vector<std::string>;
    using DbTable = std::vector<DbRow>;
    
public:
    void SetAdapter(const DbAdapterBase& adapter) override;

    void Run(const std::string& connectionString) override;

    bool InsertUser(const std::string& login, const std::string& password) const override;

    [[nodiscard]] std::string GetPassword(const std::string& login) const override;
};
} // namespace inklink::db_controller
