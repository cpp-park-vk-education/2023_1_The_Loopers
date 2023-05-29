#include "log_in_handler.h"

namespace inklink::auth_handler
{
bool LogInHandler::Handle(const std::string& login, const std::string& password) const {
    std::string saltAndPassword = m_dbController.GetPassword(login);
    std::string passwordInDb{};
    std::string saltInDb{};

    int i = 0;
    while (saltAndPassword[i] != " ")
    {
        saltInDb += saltAndPassword[i];
        ++i;
    }

    while (i < saltAndPassword.size())
    {
        passwordInDb += saltAndPassword[i];
        ++i;
    }
    std::string hashedPassword = m_encrypter.EncryptWithSalt(password, saltInDb);

    if (hashedPassword == passwordInDb)
    {
        return true;
    }

    return false;
}
} // namespace inklink::auth_handler
