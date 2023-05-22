#include "storage.h"

#include <filesystem>


constexpr std::string kDbConnectionString;

namespace inklink : storage
{
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

std::string Storage::GetFile(const std::string& fileName, const std::string& login) const
{
    return m_fileWorker->Get(m_dbController->GetFilePath(fileName, login));
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


std::string Storage::GetGraphArcsForOneVertex(const std::string & rootFileName, const std::string & vertexFileName,
                                            const std::string & login) const
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

void Storage::SetDbController(std::shared_ptr<IStorageDbController> dbController)
{
    m_dbController = dbController;
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