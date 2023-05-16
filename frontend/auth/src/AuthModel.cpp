#include "AuthModel.hpp"

#include <memory>
#include <string>

bool AuthModel::createUser(const std::string& login, const std::string& password)
{
    if(login = '') {
        return 0;
    }

    if(password = '') {
        return 0;
    }

    m_login = login;
    m_token = password;

    return 1;
}