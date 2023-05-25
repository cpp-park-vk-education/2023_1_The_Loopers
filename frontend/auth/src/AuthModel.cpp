#include "AuthModel.hpp"

#include <string>

namespace inklink::auth
{
bool AuthModel::CreateUser(const std::string& login, std::string& password)
{
    //        std::string sault{};
    //        sault = dinamicSault(password.size());
    //
    //        password += sault;
    //        password = sha256(password);

    auto result = ParserToJson(login, password);
    //        result = ParserToJson(login, password, sault);
    // осталось отправить на web-socket
    return true;
}

bool AuthModel::Login(const std::string& login, const std::string& password)
{
    // отправляю логин и пароль на веб-сокет
    // получаю true/false и пускаю/не пускаю пользователя
}

void AuthModel::Exit()
{
    //???
}

const std::string AuthModel::GetLogin() const
{
    return m_login;
}

const std::string AuthModel::GetToken() const
{
    return m_token;
}

void AuthModel::SetLogin(const std::string& login)
{
    m_login = login;
}

void AuthModel::SetToken(const std::string& token)
{
    m_token = token;
}

// std::string AuthModel::DinamicSault(int passwordLength)
//{
//     std::string result{};
//
//     auto saultLength = 20 - passwordLength;
//
//     srand(time(NULL));
//
//     for (int i = 0; i < saultLength; i++)
//     {
//         auto ch = 'a' + rand() % 26;
//         result.push_back(ch);
//     }
//
//     return result;
// }

// std::string AuthModel::parserToJson(const std::string& login, const std::string& password, const std::string& sault)
//{
//     std::string result{};
//     result = "{\n" + "\"login\": " + login + ",\n" + "\"password\": " + password + ",\n" + "\"sault\": " + sault +
//              "\n" + "}";
//        return result;
// }

std::string AuthModel::ParserToJson(const std::string& login, const std::string& password)
{
    std::string result = "{\n\"login\": " + login + ",\n\"password\": " + password + "\n" + "}";
    return result;
}
} // namespace inklink::auth
