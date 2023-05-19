#include "AuthModel.hpp"

#include <memory>
#include <string>

bool AuthModel::createUser(const std::string& login, const std::string& password)
{
    if (!login.empty() && !password.empty())
    {
        std::string sault;
        sault = Sault();

        password += sault;
        password = Hash(password);

        result = ParserToJson(login, password, sault);
        NetworkClient.POST() // надо передать result
    }
}

bool AuthModel::login(const std::string& login, const std::string& password)
{
    if (login != m_login)
    {
        return false;
    }

    if (password != m_token)
    {
        return false;
    }

    return true;
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

std::string ParserToJson(const std::string& login, const std::string& password, const std::string& sault)
{
std:
    string result;
    result = "{\n" + "\"login\": " + login + ",\n" + "\"password\": " + password + ",\n" + "\"sault\": " + sault +
             "\n" + "}";
}