#include "hash_and_salt.h"
#include "log_in_handler.h"

namespace inklink::auth_handler
{
bool LogInHandler::handleCredentials(const std::string& login, const std::string& password) const
{
    HashAndSalt hashAndSalt = m_dbController->GetPasswordAndSalt(login);

    std::string hashedPassword = m_encrypter.EncryptWithSalt(password, hashAndSalt.salt);

    return hashedPassword == hashAndSalt.hash;
}
} // namespace inklink::auth_handler
