#include "AuthModel.hpp"

#include "AuthView.hpp"
#include "LoginView.hpp"

namespace
{
using DataContainer = inklink::serializer::DataContainer;
}

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
    auto session = std::make_shared<WebsocketClientSession<decltype(lamOnAccept), decltype(lamOnRead)>>(
            m_ioContext, lamOnAccept, lamOnRead);
    session->RunAsync("127.0.0.1", 3994);
    m_session = session;

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
std::string AuthModel::LoginParseToSend(std::string& login, std::string& password)
{
    DataContainer sendContainer{};
    sendContainer["new"] = 0;
    sendContainer["login"] = login;
    sendContainer["password"] = password;
    return JsonSerializer::SerializeAsString(sendContainer);
}
std::string AuthModel::RegParseToSend(std::string login, std::string password)
{
    DataContainer sendContainer{};
    sendContainer["new"] = 1;
    sendContainer["login"] = std::move(login);
    sendContainer["password"] = std::move(password);
    return JsonSerializer::SerializeAsString(sendContainer);
}
void AuthModel::LoginSend(const std::string& message)
{
    std::shared_ptr<IClientSession> session = m_session.lock();
    if (!session) [[unlikely]]
    {
        auto lamOnAccept = [this](ConnectType, error_code ec, IClientSession*) { ; };
        auto lamOnRead = [this](const std::string& str, error_code ec, IClientSession*) { this->LoginParseToGet(message); };
        session = std::make_shared<WebsocketClientSession<decltype(lamOnAccept), decltype(lamOnRead)>>(
                m_ioContext, lamOnAccept, lamOnRead);

        session->RunAsync("127.0.0.1", 3994);
        m_session = session;
    }
    session->Send(message);
}
void AuthModel::RegSend(const std::string& message)
{
    std::shared_ptr<IClientSession> session = m_session.lock();
    if (!session) [[unlikely]]
    {
        auto lamOnAccept = [this](ConnectType, error_code ec, IClientSession*) { ; };
        auto lamOnRead = [this](const std::string& str, error_code ec, IClientSession*) { this->RegParseToGet(message); };
        session = std::make_shared<WebsocketClientSession<decltype(lamOnAccept), decltype(lamOnRead)>>(
                m_ioContext, lamOnAccept, lamOnRead);

        session->RunAsync("127.0.0.1", 3994);
        m_session = session;
    }
    session->Send(message);
}
void AuthModel::LoginParseToGet(std::string& webSocketData)
{
    DataContainer getContainer{};
    getContainer = JsonSerializer::ParseFromString(webSocketData);
    m_loginview->NotifyGotResultFromNetwork(true);
    return;
}
void AuthModel::RegParseToGet(std::string& webSocketData)
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