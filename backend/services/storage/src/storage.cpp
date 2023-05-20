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

[[nodiscard]] std::string Storage::GetFile(std::string& fileName, std::string& login) const
{
    return m_fileWorker->Get(m_dbController->GetFilePath(fileName, login));
}

[[nodiscard]] std::string Storage::GetAllFilesNames(std::string & login) const
{
    return m_dbController->GetAllFilesForUser(login);
}

[[nodiscard]] bool Storage::Update(std::string& fileName, std::string& login, std::string& fileChanges) const
{
    if (m_dbController->GetFilePath(fileName, login) == "")
    {
        Create(fileName, login);
    }

    m_fileWorker->Save(m_dbController->GetFilePath(fileName, login), fileChanges);
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