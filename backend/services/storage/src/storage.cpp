#include "storage.h"

#include <memory>
#include <string>
#include <experimental/filesystem>
#include <filesystem>


namespace inklink : storage
{
using filesystem::path = std::experimental::filesystem::path;
using IFileHolder = file_holder::IFileHolder;
using IStorageDbController = db_controller::IStorageDbController;
using IExternalServiceChassis = external_service_chassis:IExternalServiceChassis;

std::string Storage::GetFile(std::string& fileName, std::string& login) const
{
    return m_fileWorker->Get(m_dbController->GetFilePath(fileName, login));
}

std::string Storage::GetAllFilesNames(std::string & login) const
{
    return m_dbController->GetAllFilesForUser(login);
}

bool Storage::Update(std::string& fileName, std::string& login, std::string& fileChanges) const
{
    if (m_dbController->GetFilePath(fileName, login) == "")
    {
        Create(fileName, login);
    }

    m_fileWorker->Save(m_dbController->GetFilePath(fileName, login), fileChanges);
}


std::string Storage::GetGraphArcsForOneVertex(std::string & rootFileName, std::string & vertexFileName,
                                            std::string & login) const
{
    return m_dbController->GetGraphArcs(rootFileName, vertexFileName, login);
}

void Storage::SaveGraphArc(std::string & rootFileName, std::string & fromFileName, std::string & toFileName) const
{
    m_dbController->InsertGraphArc(rootFileName, fromFileName, toFileName);
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