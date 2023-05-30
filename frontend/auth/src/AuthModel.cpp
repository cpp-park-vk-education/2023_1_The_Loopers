#include "AuthModel.h"

#include "AuthView.h"
#include "LoginView.h"

#include <websocket_client_session.h>

#include <json_serializer.h>

#include <iostream>

namespace
{
using DataContainer = inklink::serializer::DataContainer;
using JsonSerializer = inklink::serializer::JsonSerializer;
using ConnectType = inklink::client_connector::ConnectType;
} // namespace

namespace inklink::auth
{
AuthModel::AuthModel()
{
    auto lamOnAccept = [this](ConnectType, error_code ec, IClientSession*) { ; };
    auto lamOnRead = [this](const std::string& str, error_code ec, IClientSession*)
    {
        DataContainer data = JsonSerializer::ParseFromString(str);
        if (!data.Has("new"))
        {
            return;
        }
        switch (data.AsInt("new"))
        {
        case 0:
            this->RegParseToGet(str);
            break;
        case 1:
            this->LoginParseToGet(str);
            break;
        default:
            return;
        }
    };
    auto session = std::make_shared<
            inklink::client_connector::WebsocketClientSession<decltype(lamOnAccept), decltype(lamOnRead)>>(
            m_ioContext, lamOnAccept, lamOnRead);
    session->RunAsync("127.0.0.1", 3994);
    m_session = session;

    std::cout << "AuthModel     " << __LINE__ << std::endl;

    m_ioContextExecutor =
            boost::asio::require(m_ioContext.get_executor(), boost::asio::execution::outstanding_work.tracked);

    m_thread_ioContext = std::thread([this]() { this->m_ioContext.run(); });
}

AuthModel::~AuthModel()
{
    m_ioContext.stop();

    m_thread_ioContext.join();
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
    std::lock_guard<std::mutex> lock(m_tokenMutex);
    return m_token;
}

void AuthModel::SetLogin(const std::string& login)
{
    m_login = login;
}

void AuthModel::SetToken(const std::string& token)
{
    std::lock_guard<std::mutex> lock(m_tokenMutex);
    m_token = token;
}

std::string AuthModel::LoginParseToSend(const std::string& login, const std::string& password)
{
    DataContainer sendContainer{};
    sendContainer["new"] = 0;
    sendContainer["login"] = login;
    sendContainer["password"] = password;
    return JsonSerializer::SerializeAsString(sendContainer);
}

std::string AuthModel::RegParseToSend(const std::string& login, const std::string& password)
{
    DataContainer sendContainer{};
    sendContainer["new"] = 1;
    sendContainer["login"] = std::move(login);
    sendContainer["password"] = std::move(password);
    return JsonSerializer::SerializeAsString(sendContainer);
}

void AuthModel::LoginSend(const std::string& message)
{
    if (m_session.expired()) [[unlikely]]
    {
        auto lamOnAccept = [this](ConnectType, error_code ec, IClientSession*) { ; };
        auto lamOnRead = [this](const std::string& str, error_code ec, IClientSession*) { this->LoginParseToGet(str); };
        session = std::make_shared<
                inklink::client_connector::WebsocketClientSession<decltype(lamOnAccept), decltype(lamOnRead)>>(
                m_ioContext, lamOnAccept, lamOnRead);

        session->RunAsync("127.0.0.1", 3994);
        m_session = session;
    }
    std::shared_ptr<IClientSession> session = m_session.lock();
    session->Send(message);
}

void AuthModel::RegSend(const std::string& message)
{
    if (m_session.expired()) [[unlikely]]
    {
        auto lamOnAccept = [this](ConnectType, error_code ec, IClientSession*) { ; };
        auto lamOnRead = [this](const std::string& str, error_code ec, IClientSession*) { this->RegParseToGet(str); };
        session = std::make_shared<
                inklink::client_connector::WebsocketClientSession<decltype(lamOnAccept), decltype(lamOnRead)>>(
                m_ioContext, lamOnAccept, lamOnRead);

        session->RunAsync("127.0.0.1", 3994);
        m_session = session;
    }
    std::shared_ptr<IClientSession> session = m_session.lock();
    session->Send(message);
}

void AuthModel::LoginParseToGet(const std::string& webSocketData)
{
    DataContainer getContainer{};
    getContainer = JsonSerializer::ParseFromString(webSocketData);
    m_loginview->NotifyGotResultFromNetwork(true);
    return;
}

void AuthModel::RegParseToGet(const std::string& webSocketData)
{
    DataContainer getContainer{};
    getContainer = JsonSerializer::ParseFromString(webSocketData);
    m_authview->NotifyGotResultFromNetwork(true);
    return;
}

void AuthModel::SaveTokenEtcForFutureUse(const DataContainer& data)
{
    if (data.AsInt("error"))
    {
        return;
    }
    SetToken(data.AsString("token"));
}
} // namespace inklink::auth
