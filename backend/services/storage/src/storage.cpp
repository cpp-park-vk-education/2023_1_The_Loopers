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

[[nodiscard]] std::string Storage::GetFile(std::string& fileName, std::string& login)
{
    return m_fileWorker->Get(m_dbController->GetFilePath(fileName, login));
}


} // namespace inklink: storage