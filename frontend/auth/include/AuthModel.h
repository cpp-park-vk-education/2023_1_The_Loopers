#pragma once

#include <data_container.h>

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

#include <chrono>
#include <mutex>
#include <string>
#include <thread>

// forward declaration
namespace inklink::client_connector
{
class IClientSession;
}

namespace inklink::auth
{
class AuthDialog;
class LoginDialog;
} // namespace inklink::auth

namespace
{
using namespace std::chrono_literals;
using error_code = boost::system::error_code;
} // namespace

namespace inklink::auth
{
class AuthModel
{
public:
    using IClientSession = inklink::client_connector::IClientSession;

public:
    AuthModel();
    ~AuthModel();
    bool CreateUser(const std::string& login, const std::string& password);
    bool Login(const std::string& login, const std::string& password);
    void Exit();
    const std::string GetLogin() const;
    const std::string GetToken() const;
    std::string LoginParseToSend(const std::string& login, const std::string& password);
    std::string RegParseToSend(const std::string& login, const std::string& password);
    void LoginSend(const std::string& message);
    void RegSend(const std::string& message);

private:
    void SetLogin(const std::string& login);
    void SetToken(const std::string& token);
    void LoginParseToGet(const std::string& webSocketData);
    void RegParseToGet(const std::string& webSocketData);
    void SaveTokenEtcForFutureUse(const serializer::DataContainer& data);

private:
    AuthDialog* m_authview;

    std::weak_ptr<IClientSession> m_session;

    boost::asio::io_context m_ioContext;

    boost::asio::any_io_executor m_ioContextExecutor;

    std::thread m_thread_ioContext;

    mutable std::mutex m_tokenMutex;
    std::string m_token;
    std::string m_login;
};
} // namespace inklink::auth
