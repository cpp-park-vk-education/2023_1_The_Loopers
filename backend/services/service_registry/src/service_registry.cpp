#include "service_registry.h"

#include "inklink_global.h"

#include <data_container.h>
#include <iauthorizer.h>
#include <ibase_service_chassis.h>
#include <inklink/chassis_configurators/base_websocket_configurator.h>
#include <iservice_session.h>
#include <websocket_session_factory.h>

#include <json_serializer.h>

#include <algorithm>
#include <exception>
#include <stdexcept>

namespace
{
using IBaseServiceChassis = inklink::base_service_chassis::IBaseServiceChassis;
using InternalSessionsManager = inklink::base_service_chassis::InternalSessionsManager;
using IServiceSession = inklink::server_network::IServiceSession;
using IAuthorizer = inklink::authorizer::IAuthorizer;
using DataContainer = inklink::serializer::DataContainer;
using JsonSerializer = inklink::serializer::JsonSerializer;

namespace net = boost::asio;
using tcp = net::ip::tcp;
using error_code = boost::system::error_code;

using time_point = std::chrono::time_point<std::chrono::system_clock>;

// comments as in simultaneous access
constexpr const char* kLogPathPrefix = "inklink/service_registry/";

class InvalidMessageException
{
};

DataContainer CreateServiceList(inklink::ServiceType type, const std::vector<inklink::Endpoint>& endpoints)
{
    DataContainer result{};
    result["service_type"] = static_cast<int>(type);
    auto& serviceListArray = result["service_list"].CreateArray();
    for (const auto& endpoint : endpoints)
    {
        DataContainer service{};
        service["address"] = endpoint.address;
        service["port"] = static_cast<int>(endpoint.port);
        serviceListArray.push_back(service);
    }
    return result;
}
} // namespace

namespace inklink::service_registry
{

int ServiceRegistry::Run()
{
    boost::asio::io_context ioContext{};

    auto manager = std::make_shared<InternalSessionsManager>();
    auto authorizer = std::make_shared<IAuthorizer>();
    auto onReadFunctor = [this](const std::string& str, error_code ec, IServiceSession* iss)
    { DoOnRead(str, ec, iss); };
    auto onConnectFunctor = [this](error_code ec, IServiceSession* iss) { DoOnConnect(ec, iss); };
    auto onWriteFunctor = [this](error_code ec, IServiceSession* iss) { DoOnWrite(ec, iss); };

    auto factory = std::make_unique<server_network::WebsocketSessionsFactory<
            decltype(onReadFunctor), decltype(onConnectFunctor), decltype(onWriteFunctor)>>( // I think, it's ok with
                                                                                             // default template params
            manager, authorizer, onReadFunctor, onConnectFunctor, onWriteFunctor);

    std::filesystem::create_directories(kLogPathPrefix);
    // TODO (a.novak) add time to file name. For some reason, std format does not work
    //     const auto startTime = std::chrono::system_clock::now();
    const std::string logPath{std::string(kLogPathPrefix) + "service_registry_.txt"};
    //      + std::format("{:%Y_%m_%d_%H_%M}", startTime) + ".txt"};
    // clang-format off
    m_chassis = chassis_configurator::BaseChassisWebsocketConfigurator::
            CreateAndInitializeChassisWithoutRegistratorAndMsgBroker("service registry", logPath, 
                                                                      ioContext, std::move(factory), manager,
                                                                     {.address = m_address, .port = m_port});
    // clang-format on
    m_chassis->logger->LogInfo("Service registry service is initted");

    // for now only one thread is supported
    ioContext.run();
    return 0;
}

void ServiceRegistry::DoOnRead(const std::string& msg, error_code ec, IServiceSession* session)
{
    if (ec)
    {
        m_chassis->logger->LogWarning("Got error while reading");
        return;
    }

    const auto msgData = JsonSerializer::ParseFromString(msg);

    try
    {
        if (!msgData.Has("action_type") || !msgData.Has("description") || !msgData["description"].Has("service_type"))
                [[unlikely]]
        {
            throw InvalidMessageException();
        }

        DataContainer replyMsg{};
        switch (msgData.AsInt("action_type"))
        {
        case RegistryActionType::kRegister:
            replyMsg = HandleRegisterQuery(msgData);
            break;
        case RegistryActionType::kExit:
            replyMsg = HandleExitQuery(msgData);
            break;
        case RegistryActionType::kGetServicesList:
            replyMsg = HandleGetServiceListQuery(msgData);
            break;
        default:
            throw InvalidMessageException();
        }

        session->Send(serializer::JsonSerializer::SerializeAsString(replyMsg));
    }
    catch (const InvalidMessageException&)
    {
        m_chassis->logger->LogWarning(std::string("Got invalid message: ") + msg);

        DataContainer replyMsg{};
        replyMsg["action_result"] = 0;
        replyMsg["time"] = std::string("now"); // TODO (a.novak) serialize std::time_point
        session->Send(serializer::JsonSerializer::SerializeAsString(replyMsg));
    }
}

void ServiceRegistry::DoOnConnect(error_code ec, IServiceSession*)
{
    if (ec)
    {
        m_chassis->logger->LogDebug(std::string("Got error from ... when tried to accept. Error: ") + ec.what());
        return;
    }
}

void ServiceRegistry::DoOnWrite(error_code ec, IServiceSession*)
{
    if (ec)
    {
        m_chassis->logger->LogDebug(std::string("Got error from ... while writing. Error: ") + ec.what());
        return;
    }
}

DataContainer ServiceRegistry::HandleRegisterQuery(const DataContainer& msgData)
{
    if (!msgData["description"].Has("endpoint"))
    {
        throw InvalidMessageException();
    }

    const Endpoint endpoint = {.address = msgData["description"]["endpoint"].AsString("address"),
                               .port = static_cast<std::uint16_t>(msgData["description"]["endpoint"].AsInt("port"))};
    switch (msgData["description"].AsInt("service_type"))
    {
    case 0:
        m_services[ServiceType::kServiceRegistry].push_back(endpoint);
        break;
    case 1:
        m_services[ServiceType::kMessageBroker].push_back(endpoint);
        break;
    case 2:
        m_services[ServiceType::kApiGateway].push_back(endpoint);
        break;
    case 3:
        m_services[ServiceType::kAuth].push_back(endpoint);
        break;
    case 4:
        m_services[ServiceType::kFileStorage].push_back(endpoint);
        break;
    case 5:
        m_services[ServiceType::kSimultaneousAccess].push_back(endpoint);
        break;
    default:
        throw InvalidMessageException();
    }

    DataContainer replyMsg{};
    replyMsg["action_result"] = 1;
    replyMsg["time"] = std::string("now"); // TODO (a.novak) serialize std::time_point
    return replyMsg;
}

DataContainer ServiceRegistry::HandleExitQuery(const DataContainer& msgData)
{
    if (!msgData["description"].Has("endpoint"))
    {
        throw InvalidMessageException();
    }

    const Endpoint endpoint{.address = msgData["description"]["endpoint"].AsString("address"),
                            .port = static_cast<std::uint16_t>(msgData["description"]["endpoint"].AsInt("port"))};
    switch (msgData["description"].AsInt("service_type"))
    {
    case 0:
        std::erase(m_services[ServiceType::kServiceRegistry], endpoint);
        break;
    case 1:
        std::erase(m_services[ServiceType::kMessageBroker], endpoint);
        break;
    case 2:
        std::erase(m_services[ServiceType::kApiGateway], endpoint);
        break;
    case 3:
        std::erase(m_services[ServiceType::kAuth], endpoint);
        break;
    case 4:
        std::erase(m_services[ServiceType::kFileStorage], endpoint);
        break;
    case 5:
        std::erase(m_services[ServiceType::kSimultaneousAccess], endpoint);
        break;
    default:
        throw InvalidMessageException();
    }

    DataContainer replyMsg{};
    replyMsg["action_result"] = 1;
    replyMsg["time"] = std::string("now"); // TODO (a.novak) serialize std::time_point
    return replyMsg;
}

DataContainer ServiceRegistry::HandleGetServiceListQuery(const DataContainer& msgData)
{
    DataContainer replyMsg{};
    replyMsg["time"] = std::string("now");
    auto& services = replyMsg["services"].CreateArray();

    switch (msgData["description"].AsInt("service_type"))
    {
    case 0:
        services.push_back(CreateServiceList(ServiceType::kServiceRegistry, m_services[ServiceType::kServiceRegistry]));
        break;
    case 1:
        services.push_back(CreateServiceList(ServiceType::kMessageBroker, m_services[ServiceType::kMessageBroker]));
        break;
    case 2:
        services.push_back(CreateServiceList(ServiceType::kApiGateway, m_services[ServiceType::kApiGateway]));
        break;
    case 3:
        services.push_back(CreateServiceList(ServiceType::kAuth, m_services[ServiceType::kAuth]));
        break;
    case 4:
        services.push_back(CreateServiceList(ServiceType::kFileStorage, m_services[ServiceType::kFileStorage]));
        break;
    case 5:
        services.push_back(
                CreateServiceList(ServiceType::kSimultaneousAccess, m_services[ServiceType::kSimultaneousAccess]));
        break;
    case 100:
        services.push_back(CreateServiceList(ServiceType::kServiceRegistry, m_services[ServiceType::kServiceRegistry]));
        services.push_back(CreateServiceList(ServiceType::kMessageBroker, m_services[ServiceType::kMessageBroker]));
        services.push_back(CreateServiceList(ServiceType::kApiGateway, m_services[ServiceType::kApiGateway]));
        services.push_back(CreateServiceList(ServiceType::kAuth, m_services[ServiceType::kAuth]));
        services.push_back(CreateServiceList(ServiceType::kFileStorage, m_services[ServiceType::kFileStorage]));
        services.push_back(
                CreateServiceList(ServiceType::kSimultaneousAccess, m_services[ServiceType::kSimultaneousAccess]));
        break;
    default:
        throw InvalidMessageException();
    }

    return replyMsg;
}
} // namespace inklink::service_registry
