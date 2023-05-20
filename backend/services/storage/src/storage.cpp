#include "storage.h"

#include <memory>
#include <string>
#include <experimental/filesystem>
#include <filesystem>


constexpr std::string kDbConnectionString;

namespace inklink : storage
{
using filesystem::path = std::experimental::filesystem::path;
using IFileHolder = file_holder::IFileHolder;
using IStorageDbController = db_controller::IStorageDbController;
using IExternalServiceChassis = external_service_chassis:IExternalServiceChassis;

void Run(int port)
{
    try
    {
        m_dbController->Connect(kDbConnectionString);
    }
    catch (const std::exception&)
    {
    }
}

std::string Storage::GetFile(std::string& fileName, std::string& login) const
{
    return m_fileWorker->Get(m_dbController->GetFilePath(fileName, login));
}

std::string Storage::GetAllFilesNames(std::string & login) const
{
    try
    {
        return m_dbController->GetAllFilesForUser(login);
    }
    catch (const std::exception&)
    {
    }
}

bool Storage::Update(std::string& fileName, std::string& login, std::string& fileChanges) const
{
    try
    {
        if (m_dbController->GetFilePath(fileName, login) == "")
        {
            Create(fileName, login);
        }

        m_fileWorker->Save(m_dbController->GetFilePath(fileName, login), fileChanges);
    }
    catch (const std::exception&)
    {
    }
}


std::string Storage::GetGraphArcsForOneVertex(std::string & rootFileName, std::string & vertexFileName,
                                            std::string & login) const
{
    try
    {
        return m_dbController->GetGraphArcs(rootFileName, vertexFileName, login);
    }
    catch (const std::exception&)
    {
    }
}

void Storage::SaveGraphArc(std::string& rootFileName, std::string& fromFileName, std::string& toFileName) const
{
    try
    {
        m_dbController->InsertGraphArc(rootFileName, fromFileName, toFileName);
    }
    catch (const std::exception&)
    {
    }
}

void Storage::SetChassis(std::shared_ptr<IExternalServiceChassis> serviceChassis)
{
    m_serviceChassis = serviceChassis;
}

void Storage::SetDbController(std::shared_ptr<IStorageDbController> dbController)
{
    m_dbController = dbController;
}

void Storage::SetFileHolder(std::shared_ptr<IFileHolder> fileHolder)
{
    m_fileWorker = fileHolder;
}


[[nodiscard]] bool Storage::Create(std::string & fileName, std::string & login)
{
    filesystem::path filePath = "files/" + login + "/" + fileName + ".txt";

    m_dbController->InsertFile(fileName, login, filePath);
}
} // namespace inklink: storage