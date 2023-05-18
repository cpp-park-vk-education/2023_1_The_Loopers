#include "AuthModel.hpp"

#include <memory>
#include <string>

bool AuthModel::createUser(const std::string& login, const std::string& password)
{
    login.empty();

    password.empty();

    m_login = login;
    m_token = password;

    return true;
}

bool AuthModel::login(const std::string& login, const std::string& password)
{
    if (login != m_login)
    {
        return 0;
    }

    if (password != m_token)
    {
        return 0;
    }

    return 1;
}

void AuthModel::exit()
{
}

const std::string AuthModel::getLogin() const
{
    return m_login;
}

const std::string AuthModel::getToken() const
{
    return m_token;
}

void AuthModel::setLogin(const std::string& login)
{
    m_login = login;
}

void AuthModel::setToken(const std::string& token)
{
    m_token = token;
}

std::string AuthModel::encrypt(const std::string& password)
{
    return m_enncrypter.encryptString(password);
}

