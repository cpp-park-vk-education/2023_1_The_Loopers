#pragma once

#include "data_container.h"

#include <memory>
#include <string>

namespace inklink::auth
{
class AuthModel
{
    using DataContainer = inklink::serializer::DataContainer;

public:
    bool CreateUser(std::string& login, std::string& password);
    bool Login(std::string& login, std::string& password);
    void Exit();
    const std::string GetLogin() const;
    const std::string GetToken() const;

protected:
    void SetLogin(const std::string& login);
    void SetToken(const std::string& token);
    //    static std::string DinamicSault(int passwordLength);
    std::string ParseToSend(std::string& login, std::string& password);
    void ParseToGet(std::string& webSocketData);

private:
    std::string m_login;
    std::string m_token;
};
} // namespace inklink::auth
