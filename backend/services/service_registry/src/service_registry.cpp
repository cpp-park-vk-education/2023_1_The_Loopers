#include "service_registry.h"

#include <ibase_service_chassis.h>

namespace
{
using IBaseServiceChassis = inklink::base_service_chassis::IBaseServiceChassis;
using IExternalServiceChassis = inklink::external_service_chassis::IExternalServiceChassis;
using InternalSessionsManager = inklink::base_service_chassis::InternalSessionsManager;
using IServiceSession = inklink::server_network::IServiceSession;
using IAuthorizer = inklink::server_network::IAuthorizer;

namespace net = boost::asio;
using tcp = net::ip::tcp;
using error_code = boost::system::error_code;

using time_point = std::chrono::time_point<std::chrono::system_clock>;

// comments as in simultaneous access
constexpr const char* kLogPathPrefix = "inklink/service_registry/";
} // namespace

namespace inklink::service_registry
{

int ServiceRegistry::Run()
{
    boost::asio::io_context ioContext;

    auto manager = std::make_shared<InternalSessionsManager>();
    auto auhorizer = std::make_shared<IAuthorizer>();
    auto factory = std::make_unique<WebsocketSessionsFactory>( // I think, it's ok with default template params
            manager, authorizer,
            [this](const std::string& str, error_code ec, IServiceSession* iss) { DoOnRead(str, ec, iss); },
            [this](error_code ec, IServiceSession* iss) { DoOnConnect(ec, iss); },
            [this](error_code ec, IServiceSession* iss) { DoOnWrite(ec, iss); });

    // TODO (a.novak) intellisense gives errors, but should not. Test with build in future (for now there is
    // dependencies on files from not merged prs)
    std::filesystem::create_directories(kLogPathPrefix);
    // TODO (a.novak) add time to file name. For some reason, std format does not work
    //     const auto startTime = std::chrono::system_clock::now();
    const std::string logPath{std::string(kLogPathPrefix) + "service_registry_.txt"};
    //      + std::format("{:%Y_%m_%d_%H_%M}", startTime) + ".txt"};
    // clang-format off
    m_chassis = base_service_chassis::BaseChassisWebsocketConfigurator::
            CreateAndInitializeChassisWithoutRegistratorAndMsgBroker("simultaneous access", logPath, 
                                                                      ioContext, std::move(factory), manager,
                                                                     {.address = m_address, .port = m_port});
    // clang-format on
    m_chassis->logger->LogInfo("Service registry service is initted");

    // for now only one thread is supported
    ioContext.run();
    return 0;
}

void ServiceRegistry::DoOnRead(const std::string&, error_code, IServiceSession*)
{
    // TODO
    // m_services[].push_back();
}

void ServiceRegistry::DoOnConnect(error_code ec, IServiceSession*)
{
    if (ec)
    {
        m_chassis->logger->LogDebug(std::string("Got error from ... when tried to accept. Error: ") + ec.what());
    }
}

void ServiceRegistry::DoOnWrite(error_code, IServiceSession*)
{
    if (ec)
    {
        m_chassis->logger->LogDebug(std::string("Got error from ... while writing. Error: ") + ec.what());
    }
}
} // namespace inklink::service_registry
