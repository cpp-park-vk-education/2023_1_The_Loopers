#include "log_in_handler.h"

#include

namespace inklink::auth_handler
{
bool LogInHandler::Handle(const std::string& login, const std::string& password) const {
    std::string passwordInDb = m_dbController.GetPassword(login);
    unsigned char* hashedPassword = m_encrypter.Encrypt(password);
    if (passwordInDb.size() != SHA256_DIGEST_LENGTH)
    {
        return false;
    }
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
    {
        if (passwordInDb[i] != hashedPassword[i])
        {
            return false;
        }
    }
    return true;
}
} // namespace inklink::auth_handler
