#include "create_handler.h"

namespace inklink::auth_handler
{
bool CreateHandler::Handle(const std::string& login, const std::string& password) const
{
    if (CheckDuplicate(login))
    {
        return false;
    }

    HashAndSalt hashAndSalt = m_encrypter.Encrypt(password);

    m_dbController.InsertUser(login, hashAndSalt.first, hashAndSalt.second);
    return true;
}

bool CreateHandler::CheckDuplicate(const std::string& login) const
{
    if (m_dbController.GetPassword(login).string().empty())
    {
        return false;
    }
    return true;
}
} // namespace inklink::auth_handler
