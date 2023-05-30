#include "create_handler.h"

namespace inklink::auth_handler
{
bool CreateHandler::handleCredentials(const std::string& login, const std::string& password) const
{
    if (!CheckIfUnique(login))
    {
        return false;
    }

    HashAndSalt hashAndSalt = m_encrypter.Encrypt(password);

    m_dbController->InsertUser(login, hashAndSalt.hash, hashAndSalt.salt);
    return true;
}

bool CreateHandler::CheckIfUnique(const std::string& login) const
{
    return m_dbController->FindUser(login).string().empty();
}
} // namespace inklink::auth_handler
