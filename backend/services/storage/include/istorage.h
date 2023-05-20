#ifndef _BACKEND_SERVICES_STORAGE_INCLUDE_ISTORAGE_H_
#define _BACKEND_SERVICES_STORAGE_INCLUDE_ISTORAGE_H_

#include "ifile_holder.h"
#include "idb_adapter"
#include "iexternal_service_chassis.h"

#include <memory>
#include <string>
#include <experimental/filesystem>
#include <filesystem>


namespace inklink:storage
{
class IStorage
{
using filesystem::path = std::experimental::filesystem::path;
using IFileHolder = file_holder::IFileHolder;
using IStorageDbController = db_controller::IStorageDbController;
using IExternalServiceChassis = external_service_chassis:IExternalServiceChassis;

public:
    virtual [[nodiscard]] std::string GetFile(std::string& fileName, std::string& login) = 0;
    virtual [[nodiscard]] bool Update(std::string& fileName, std::string& login, std::string& fileChanges) = 0;


    virtual void SetChassis(std::shared_ptr<IExternalServiceChassis> serviceChassis) = 0;
    virtual void SetDbController(std::shared_ptr<IStorageDbController> dbController) = 0;
    virtual void SetFileHolder(std::shared_ptr<IFileHolder> fileHolder) = 0;

protected:
    virtual [[nodiscard]] bool Create(std::string& fileName, std::string& login) = 0;

    std::shared_ptr<IFileHolder> m_fileWorker;
    std::shared_ptr<IStorageDbController> m_dbController;
    std::shared_ptr<IExternalServiceChassis> m_serviceChassis;
};
} // namespace inklink:storage
#endif // _BACKEND_SERVICES_STORAGE_INCLUDE_ISTORAGE_H_
