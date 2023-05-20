#ifndef _BACKEND_SERVICES_STORAGE_INCLUDE_STORAGE_H_
#define _BACKEND_SERVICES_STORAGE_INCLUDE_STORAGE_H_

#include "istorage.h"

#include <memory>
#include <string>
#include <experimental/filesystem>
#include <filesystem>


namespace inklink : storage
{
class Storage final : public IStorage
{
using filesystem::path = std::experimental::filesystem::path;
using IFileHolder = file_holder::IFileHolder;
using IStorageDbController = db_controller::IStorageDbController;
using IExternalServiceChassis = external_service_chassis:IExternalServiceChassis;

public:
    void Run(int port) override;
    [[nodiscard]] std::string GetFile(std::string& fileName, std::string& login) const override;
    [[nodiscard]] bool Update(std::string& fileName, std::string& login, std::string& fileChanges) const override;

    void SetChassis(std::shared_ptr<IExternalServiceChassis> serviceChassis)  override;
    void SetDbController(std::shared_ptr<IStorageDbController> dbController) override;
    void SetFileHolder(std::shared_ptr<IFileHolder> fileHolder) override;


    [[nodiscard]] std::string GetAllFilesNames(std::string& login) const override;
    [[nodiscard]] std::string GetGraphArcsForOneVertex(std::string& rootFileName, std::string& vertexFileName,
                                                       std::string& login) const override;

private:
    [[nodiscard]] bool Create(std::string& fileName, std::string& login) const override;
};
} // namespace inklink:storage
#endif // _BACKEND_SERVICES_STORAGE_INCLUDE_STORAGE_H_
