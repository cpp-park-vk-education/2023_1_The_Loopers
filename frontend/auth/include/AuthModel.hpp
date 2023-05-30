#pragma once

#include <websocket_client_session.h>
#include <data_container.h>
#include "json_serializer.h"

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

#include <chrono>
#include <mutex>
#include <string>
#include <thread>

// forward declaration
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
    std::string LoginParseToSend(std::string& login, std::string& password);
    std::string RegParseToSend(std::string login, std::string password);
    void LoginSend(std::string message);
    void RegSend(std::string message);

private:
    void SetLogin(const std::string& login);
    void SetToken(const std::string& token);
    void LoginParseToGet(std::string& webSocketData);
    void RegParseToGet(std::string& webSocketData);
    void SaveTokenEtcForFutureUse(const serializer::DataContainer& data);

private:
    AuthDialog* m_authview;
    LoginDialog* m_loginview;

    std::weak_ptr<IClientSession> m_session;

    boost::asio::io_context m_ioContext;

    boost::asio::any_io_executor m_ioContextExecutor;

    std::thread m_thread_ioContext;

    mutable std::mutex m_tokenMutex;
    std::string m_token;
    std::string m_login;
};
} // namespace inklink::auth
