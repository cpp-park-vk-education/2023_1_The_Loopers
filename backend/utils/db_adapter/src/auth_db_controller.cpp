#include "auth_db_controller.h"

#include <memory>


namespace inklink::db_controller
{
void SetAdapter(const DbAdapterBase& adapter)
{
    m_adapter = adapter;
}

void Run(const std::string& connectionString)
{
    m_adapter.Connect(connectionString);

     std::shared_ptr<pqxx::connection> settings = m_adapter.GetConnection();

     settings->prepare("InsertUser", "INSERT INTO Users Values($1, $2)");
     settings->prepare("GetPassword", "SELECT password FROM Users WHERE login = $1");
}

bool InsertUser(const std::string& login, const std::string& password) const
{
    if (login.empty() || password.empty())
    {
        return false;
    }

    m_adapter.Insert("InsertFile", login, password);
}

[[nodiscard]] std::string GetPassword(const std::string& login) const
{
    if (login.empty())
    {
        return "";
    }

    DbTable password = m_adapter.Select("GetPassword", login);
    return password[0][0];
}
} // namespace inklink::db_controller
