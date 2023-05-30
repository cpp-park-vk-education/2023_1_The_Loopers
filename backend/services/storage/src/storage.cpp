#include "storage.h"

#include <boost/asio/io_context.hpp>

#include <filesystem>

namespace
{
constexpr std::filesystem::path kLogPath = "inklink/storage/storage_.txt";
}

namespace inklink::storage
{
int Storage::Run(int port)
{
    try
    {
        boost::asio::io_context ioContext;


        SetChassis(std::make_unique<IExternalServiceChassis>());
        SetDbController(std::make_shared<IStorageDbController>(), port);
        SetFileHolder(std::shared_ptr<IFileHolder>());


        auto manager = std::make_shared<InternalSessionsManager>();
        auto authorizer = std::make_shared<IAuthorizer>();

         auto onReadFunctor = [this](const std::string& str, error_code ec, IServiceSession* iss)
        { DoOnRead(str, ec, iss); };
        auto onConnectFunctor = [this](error_code ec, IServiceSession* iss) { DoOnConnect(ec, iss); };
        auto onWriteFunctor = [this](error_code ec, IServiceSession* iss) { DoOnWrite(ec, iss); };

        auto factory = std::make_unique<server_network::WebsocketSessionsFactory<
                decltype(onReadFunctor), decltype(onConnectFunctor), decltype(onWriteFunctor)>>(
                manager, authorizer, onReadFunctor, onConnectFunctor, onWriteFunctor);

        m_serviceChassis->baseServiceChassis =
                chassis_configurator::BaseChassisWebsocketConfigurator::CreateAndInitializeFullChassis(
                        "storage", kLogPath, ioContext, std::move(factory), manager, ServiceType::kFileStorage,
                        {.address = m_address, .port = m_port},
                        [this](int eventType, const std::string& msgBody, Endpoint from)
                        { DoOnNotified(eventType, msgBody, from); },
                        [this](const std::string& msgBody) { DoOnSignal(msgBody); });

        m_serviceChassis->baseServiceChassis->logger->LogInfo("Storage service is initted");
        std::cout << "Storage is initted" << __LINE__ << std::endl;

        ioContext.run();
        return 0;
    }
    catch (const std::exception& ec)
    {
        m_serviceChassis->baseServiceChassis->logger->LogDebug(std::string("Got error in initialazing. Error: " + ec.message());
        return -1;
    }
}

bool Storage::DoOnRead(error_code errocCode, const std::string& msg, IServiceSession* serviceSession)
{
    if (ec)
    {
        m_serviceChassis->logger->LogDebug(std::string("Got error while reading from '...'. Error: ") + ec.what());
    }

    auto msgData = JsonSerializer::ParseString(msg);
    try
    {
    }
    catch (const std::exception&)
    {
    }
}

std::string Storage::GetFile(const std::string& fileName, const std::string& login) const
{
    auto[file, isCorrect] = m_fileWorker->Get(m_dbController->GetFilePath(fileName, login));
    if (isCorrect)
    {
        return file;
    }
}

std::string Storage::GetAllFilesNames(const std::string& login) const
{
    try
    {
        return m_dbController->GetAllFilesForUser(login);
    }
    catch (const std::exception&)
    {
    }
}

bool Storage::Update(const std::string& fileName, const std::string& login, const std::string& fileChanges) const
{
    try
    {
        if (m_dbController->GetFilePath(fileName, login).string().empty())
        {
            Create(fileName, login);
        }

        m_fileWorker->Save(m_dbController->GetFilePath(fileName, login), fileChanges);
    }
    catch (const std::exception&)
    {
    }
}


std::string Storage::GetGraphArcsForOneVertex(const std::string& rootFileName, const std::string& vertexFileName,
                                              const std::string& login) const
{
    try
    {
        return m_dbController->GetGraphArcs(rootFileName, vertexFileName, login);
    }
    catch (const std::exception&)
    {
    }
}

void Storage::SaveGraphArc(const std::string& rootFileName, const std::string& fromFileName, const std::string& toFileName) const
{
    try
    {
        m_dbController->InsertGraphArc(rootFileName, fromFileName, toFileName);
    }
    catch (const std::exception&)
    {
    }
}

void DeleteFile(const std::string& fileName, const std::string& login) const
{
    m_dbController.SetFileDeleted(fileName, login);
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


[[nodiscard]] bool Storage::Create(const std::string & fileName, std::string & login)
{
    std::filesystem::path filePath = "files/" + login + "/" + fileName + ".txt";

    m_dbController->InsertFile(fileName, login, filePath);
}
} // namespace inklink: storage
