#ifndef _BACKEND_SERVICES_STORAGE_INCLUDE_ISTORAGE_H_
#define _BACKEND_SERVICES_STORAGE_INCLUDE_ISTORAGE_H_

#include "idb_adapter"
#include "iexternal_service_chassis.h"
#include "ifile_holder.h"

#include <memory>
#include <string>


namespace inklink::storage
{
class IStorage
{
public:
    using error_code = boost::system::error_code;
    using IFileHolder = file_holder::IFileHolder;
    using IStorageDbController = db_controller::IStorageDbController;
    using IExternalServiceChassis = external_service_chassis:IExternalServiceChassis;

public:
    virtual int Run(int port);
    virtual bool DoOnRead(error_code errocCode, const std::string& request, IServiceSession* serviceSession);

    virtual [[nodiscard]] std::string GetFile(std::string& fileName, std::string& login) const = 0;
    virtual [[nodiscard]] bool Update(std::string& fileName, std::string& login, std::string& fileChanges) const = 0;


    virtual void SetChassis(std::shared_ptr<IExternalServiceChassis> serviceChassis) = 0;
    virtual void SetDbController(std::shared_ptr<IStorageDbController> dbController) = 0;
    virtual void SetFileHolder(std::shared_ptr<IFileHolder> fileHolder) = 0;


    virtual [[nodiscard]] std::string GetAllFilesNameslFiles(const std::string& login) const = 0;
    virtual [[nodiscard]] std::string GetGraphArcsForOneVertex(const std::string& rootFileName, const std::string& vertexFileName,
                                                               const std::string& login) const = 0;

    virtual void SaveGraphArc(const std::string& rootFileName, const std::string& fromFileName, const std::string& toFileName) const = 0;
    
    virtual void DeleteFile(const std::string& fileName, const std::string& login) const = 0;

protected:
    virtual [[nodiscard]] bool Create(const std::string& fileName, const std::string& login) const = 0;

protected:
    std::shared_ptr<IFileHolder> m_fileWorker;
    std::shared_ptr<IStorageDbController> m_dbController;
    std::shared_ptr<IExternalServiceChassis> m_serviceChassis;
};
} // namespace inklink:storage

#endif // _BACKEND_SERVICES_STORAGE_INCLUDE_ISTORAGE_H_
