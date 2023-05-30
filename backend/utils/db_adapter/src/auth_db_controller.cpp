#include "auth_db_controller.h"

#include <memory>


namespace inklink::db_controller
{
void AuthDbController::SetAdapter(const std::shared_ptr<AuthDbAdapter>& adapter)
{
    m_adapter = adapter;
}

void AuthDbController::Run(const std::string& connectionString)
{
     m_adapter->Connect(connectionString);

     std::shared_ptr<pqxx::connection> settings = m_adapter->GetConnection();

     settings->prepare("InsertUser", "INSERT INTO Users Values($1, $2, $3)");
     settings->prepare("GetPassword", "SELECT password, salt  FROM Users WHERE login = $1");
     settings->prepare("FindUser", "SELECT login FROM Users WHERE login = $1");
}

bool AuthDbController::InsertUser(const std::string& login, const std::string& password, const std::string& salt) const
{
    if (login.empty() || password.empty())
    {
        return false;
    }

    m_adapter->Insert("InsertUser", login, password, salt);
    
    return true;
}

AuthDbController::HashAndSalt AuthDbController::GetPasswordAndSalt(const std::string& login) const
{
    if (login.empty())
    {
        return {};
    }

    DbTable passwordAndSalt = m_adapter->Select("GetPassword", login);

    return {passwordAndSalt[0][0], passwordAndSalt[0][1]};
}

std::string AuthDbController::FindUser(const std::string& login) const
{
    if (login.empty())
    {
        return {};
    }

    DbTable user = m_adapter->Select("FindUser", login);

    return user[0][0];
}
} // namespace inklink::db_controller
