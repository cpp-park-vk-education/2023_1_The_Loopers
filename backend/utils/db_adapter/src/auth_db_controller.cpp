#include "auth_db_controller.h"

namespace inklink::db_controller
{
    void AuthDbController::SetAdapter(DbAdapterBase& adapter)
    {
        m_adapter = adapter;
    }

    void AuthDbController::Run(const std::string& connectionString) override
    {
        m_adapter.Connect(connectionString);

        std::shared_ptr<pqxx::connection> settings = m_adapter.GetConnection();

        settings->prepare("InsertUser", "INSERT INTO Users VALUES($1, $2)");
        settings->prepare("GetPassword", "SELECT password FROM Users WHERE login = $1");
    }

    bool AuthDbController::InsertUser(const std::string& login, const std::string& password) const
    {
        if (login.empty() || passwor.empty())
        {
            return false;
        }

        m_adapter.Insert("InsertUser", login, password);
    }

    std::string GetPassword(const std::string& login) const override
    {
        if (login.empty())
        {
            return "";
        }

        DbTable password = m_adapter.Select("GetPassword", login);
        return password[0][0];
    }
} // namespace inklink::db_controller