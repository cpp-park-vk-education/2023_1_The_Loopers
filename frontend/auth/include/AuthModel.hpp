#pragma once

#include <websocket_client_session.h>

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

#include <chrono>
#include <mutex>
#include <string>
#include <thread>

class AuthDialog;
class LoginDialog;

namespace
{
using namespace std::chrono_literals;
using namespace inklink::client_connector;
using error_code = boost::system::error_code;
} // namespace

namespace inklink::auth
{
class AuthModel
{
public:
    AuthModel();
    ~AuthModel();
    bool createUser(const std::string& login, const std::string& password);
    bool login(const std::string& login, const std::string& password);
    void exit();
    const std::string getLogin() const;
    const std::string getToken() const;
    std::string LoginParseToSend(std::string& login, std::string& password);
    std::string RegParseToSend(std::string login, std::string password);
    void LoginSend(std::string message);
    void RegSend(std::string message);

private:
    void setLogin(const std::string& login);
    void setToken(const std::string& token);
    void LoginParseToGet(std::string& webSocketData);
    void RegParseToGet(std::string& webSocketData);
    void SaveTokenEtcForFutureUse(const DataContainer& data);

private:
    AuthView* m_authview;
    LoginView* m_loginview;

    std::weak_ptr<IClientSession> m_session;

    boost::asio::io_context m_ioContext;

    boost::asio::any_io_executor m_ioContextExecutor;

    std::thread m_thread_ioContext;

    std::mutex m_tokenMutex;
    std::string m_token;
    std::string m_login;
};
} // namespace inklink::auth