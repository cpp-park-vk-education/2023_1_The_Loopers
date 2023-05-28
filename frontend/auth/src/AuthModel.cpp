#include "AuthModel.hpp"

#include <string>

namespace
{
using DataContainer = inklink::serializer::DataContainer;
}

namespace inklink::auth
{
bool AuthModel::CreateUser(std::string& login, std::string& password)
{
    return true;
}

bool AuthModel::Login(std::string& login, std::string& password)
{
    return true;
}

void AuthModel::Exit()
{
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

//std::string ParseToSend(std::string& login, std::string& password){
//    DataContainer sendContainer{};
//    sendContainer["login"] = login;
//    sendContainer["password"] = password;
//    JsonSerializer::SerializeAsString(sendContainer);
//}

//void ParseToGet(std::string& webSocketData) {
//    DataContainer getContainer{};
//    getContainer = JsonSerializer::ParseFromString(webSocketData);
//
//    return;
//}
} // namespace inklink::auth
