#pragma once

#include <memory>
#include <string>

namespace inklink::auth
{
class AuthModel
{
public:
    bool CreateUser(const std::string& login, std::string& password);
    bool Login(const std::string& login, const std::string& password);
    void Exit();
    const std::string GetLogin() const;
    const std::string GetToken() const;

protected:
    void SetLogin(const std::string& login);
    void SetToken(const std::string& token);
//    static std::string DinamicSault(int passwordLength);
//    std::string ParserToJson(const std::string& login, const std::string& password, const std::string& sault);
    std::string ParserToJson(const std::string& login, const std::string& password);
private:
    std::string m_login;
    std::string m_token;
};
} // namespace inklink::auth
