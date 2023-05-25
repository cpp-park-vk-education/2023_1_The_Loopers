#pragma once

#include "Encrypter.hpp"

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
    std::string encrypt(const std::string& password);

private:
    std::string m_login;
    std::string m_token;
    std::unique_ptr<Encrypter> m_encrypter;
};
} // namespace inklink::auth
