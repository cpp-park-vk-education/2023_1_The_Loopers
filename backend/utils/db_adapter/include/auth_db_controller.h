#pragma once

#include "iauth_db_controller.h"


#include <string>

namespace inklink::db_controller
{
class IAuthDbController
{
public:
    void SetAdapter(DbAdapterBase& adapter) override;

    void Run(const std::string& connectionString) override;

    void InsertUser(const std::string& login, const std::string& password) override;

    std::string GetPassword(const std::string& login) override;
};
} // namespace inklink::db_controller
