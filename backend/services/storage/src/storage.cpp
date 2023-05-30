#include "storage.h"

#include <websocket_session_factory.h>
#include <inklink/chassis_configurators/base_websocket_configurator.h>
#include <json_serializer.h>

#include <boost/asio/io_context.hpp>
#include <boost/system/error_code.hpp>

#include <filesystem>

namespace
{
const std::filesystem::path kLogPath = "inklink / storage / storage_.txt ";

using InternalSessionsManager = inklink::base_service_chassis::InternalSessionsManager;
using IAuthorizer = inklink::authorizer::IAuthorizer;
using IServiceSession = inklink::server_network::IServiceSession;
using DataContainer = inklink::serializer::DataContainer;
using JsonSerializer = inklink::serializer::JsonSerializer;
}

namespace inklink::storage
{
int Storage::Run(int port)
{
    try
    {
        m_port = port;

        boost::asio::io_context ioContext;


        SetChassis(std::make_unique<IExternalServiceChassis>());
        SetDbController(std::make_shared<IStorageDbController>(), port);
        SetFileHolder(std::shared_ptr<IFileHolder>());


        auto manager = std::make_shared<InternalSessionsManager>();
        auto authorizer = std::make_shared<IAuthorizer>();

         auto onReadFunctor = [this](const std::string& str, error_code ec, IServiceSession* iss)
        { DoOnRead(ec, str, iss); };

        auto factory = std::make_unique<server_network::WebsocketSessionsFactory<decltype(onReadFunctor)>>(
                 manager, authorizer, onReadFunctor);

        m_serviceChassis->baseServiceChassis =
                chassis_configurator::BaseChassisWebsocketConfigurator::CreateAndInitializeFullChassis(
                        "storage", kLogPath, ioContext, std::move(factory), manager, ServiceType::kFileStorage,
                        {.address = m_address, .port = m_port},
                        [this](int eventType, const std::string& msgBody, Endpoint from)
                        { DoOnNotified(eventType, msgBody, from); },
                        [this](const std::string& msgBody) { DoOnSignal(msgBody); });

        m_serviceChassis->baseServiceChassis->logger->LogInfo("Storage service is initted");

        ioContext.run();
        return 0;
    }
    catch (const std::exception& ec)
    {
        m_serviceChassis->baseServiceChassis->logger->LogDebug(
                std::string("Got error in initialazing. Error: ") + ec.what());
        return -1;
    }
}

bool Storage::DoOnRead(error_code errocCode, const std::string& msg, IServiceSession* serviceSession)
{
    if (errocCode)
    {
        m_serviceChassis->baseServiceChassis->logger->LogDebug(
                std::string("Got error while reading from '...'. Error: ") + errocCode.what());
    }

    auto msgData = JsonSerializer::ParseString(msg);
    try
    {
    }
    catch (const std::exception& ec)
    {
        m_serviceChassis->baseServiceChassis->logger->LogDebug(
                std::string("Got error while reading from '...'. Error: ") + ec.what());
    }
}

std::string Storage::GetFile(const std::string& fileName, const std::string& login) const
{
    try
    {
        auto [file, isCorrect] = m_fileWorker->Get(m_dbController->GetFilePath(fileName, login));
        if (isCorrect)
        {
            DataContainer sendContainer{};

            sendContainer["file"] = file;
        }
    }
    catch (const std::exception& ec)
    {
        m_serviceChassis->baseServiceChassis->logger->LogDebug(
                std::string("Got error while reading from '...'. Error: ") + ec.what());
    }
}

std::string Storage::GetAllFilesNames(const std::string& login) const
{
    try
    {
         std::string allFiles = m_dbController->GetAllFilesForUser(login);

         DataContainer sendContainer{};

         sendContainer["files"] = allFiles;
    }
    catch (const std::exception& ec)
    {
         m_serviceChassis->baseServiceChassis->logger->LogDebug(
                 std::string("Got error while reading from '...'. Error: ") + ec.what());
    }
}

bool Storage::Update(const std::string rootFileName, const std::string& fileName, const std::string& login, const std::string& fileChanges) const
{
    try
    {
        if (m_dbController->GetFilePath(fileName, login).string().empty())
        {
            Create(fileName, login, rootFileName);
        }

        m_fileWorker->Save(m_dbController->GetFilePath(fileName, login), fileChanges);
    }
    catch (const std::exception& ec)
    {
        m_serviceChassis->baseServiceChassis->logger->LogDebug(
                std::string("Got error while reading from '...'. Error: ") + ec.what());
    }
}


std::string Storage::GetGraphArcsForOneVertex(const std::string& rootFileName, const std::string& vertexFileName,
                                              const std::string& login) const
{
    try
    {
        std::string graphArcs = m_dbController->GetGraphArcs(rootFileName, vertexFileName, login);

        DataContainer sendContainer{};

        sendContainer["graph"] = graphArcs;
    }
    catch (const std::exception& ec)
    {
        m_serviceChassis->baseServiceChassis->logger->LogDebug(
                std::string("Got error while reading from '...'. Error: ") + ec.what());
    }
}

void Storage::SaveGraphArc(const std::string& rootFileName, const std::string& fromFileName, const std::string& toFileName) const
{
    try
    {
        m_dbController->InsertGraphArc(rootFileName, fromFileName, toFileName);
    }
    catch (const std::exception& ec)
    {
        m_serviceChassis->baseServiceChassis->logger->LogDebug(
                std::string("Got error while reading from '...'. Error: ") + ec.what());
    }
}

void Storage::DeleteFile(const std::string& fileName, const std::string& login) const
{
    try
    {
        m_dbController->SetFileDeleted(fileName, login);
    }
    catch (const std::exception& ec)
    {
        m_serviceChassis->baseServiceChassis->logger->LogDebug(
                std::string("Got error while reading from '...'. Error: ") + ec.what());
    }
}

void Storage::SetChassis(std::shared_ptr<IExternalServiceChassis> serviceChassis)
{
    m_serviceChassis = serviceChassis;
}

void Storage::SetDbController(std::shared_ptr<IStorageDbController> dbController, int port)
{
    m_dbController = dbController;

    std::string connectionString{"host=localhost port =" + port +
                                 " dbname=inklink_storage_db user=postges password=alex"};
    m_dbController->Run(connectionString);
}

void Storage::SetFileHolder(std::shared_ptr<IFileHolder> fileHolder)
{
    m_fileWorker = fileHolder;
}

 bool Storage::Create(const std::string& fileName, const std::string& login, const std::string& rootFileName) const
{
    std::filesystem::path filePath = "files/" + login + "/" + fileName + ".txt";

    if (fileName == rootFileName)
    {
        m_dbController->InsertRootFile(fileName, login, filePath);
    }
    else
    {
        m_dbController->InsertNonRootFile(fileName, login, filePath);
    }
}
} // namespace inklink: storage
