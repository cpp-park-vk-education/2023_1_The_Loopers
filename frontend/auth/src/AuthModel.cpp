#include "AuthModel.hpp"

#include <openssl/sha.h>

#include <cstdlib>
#include <ctime>
#include <memory>
#include <string>

bool AuthModel::createUser(const std::string& login, const std::string& password)
{
    if (!login.empty() && !password.empty())
    {
        std::string sault{};
        sault = dinamicSault(password.size());

        password += sault;
        password = sha256(password);

        result = ParserToJson(login, password, sault);
        //осталось отправить на web-socket
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

std::string AuthModel::dinamicSault(int passwordLength)
{
        std::string result{};

        int saultLength = 20 - passwordLength;

        srand(time(NULL));

        for (int i = 0; i < saultLength; i++) {
            char ch = 'a' + rand() % 26;
            result.push_back(ch);
        }

        return result;
}

std::string AuthModel::parserToJson(const std::string& login, const std::string& password, const std::string& sault)
{
    std::string result{};
    result = "{\n" + "\"login\": " + login + ",\n" + "\"password\": " + password + ",\n" + "\"sault\": " + sault +
             "\n" + "}";
}
