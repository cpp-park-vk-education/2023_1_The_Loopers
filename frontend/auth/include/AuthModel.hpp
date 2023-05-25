#pragma once

#include <memory>
#include <string>

namespace inklink::auth
{
class AuthModel
{
public:
    bool createUser(const std::string& login, const std::string& password);
    bool login(const std::string& login, const std::string& password);
    void exit();
    const std::string getLogin() const;
    const std::string getToken() const;

protected:
    void setLogin(const std::string& login);
    void setToken(const std::string& token);

private:
    std::string m_login;
    std::string m_token;
};
} // namespace inklink::auth
