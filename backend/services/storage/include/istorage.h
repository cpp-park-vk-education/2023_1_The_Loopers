#ifndef _BACKEND_SERVICES_STORAGE_INCLUDE_ISTORAGE_H_
#define _BACKEND_SERVICES_STORAGE_INCLUDE_ISTORAGE_H_

#include "ifile_holder.h"

#include <iexternal_service_chassis.h>
#include <istorage_db_controller.h>
#include <internal_sessions_manager.h>

#include <memory>
#include <string>

namespace inklink::server_network
{
class IServiceSession;
}

namespace inklink::storage
{
class IStorage
{
public:
    using error_code = boost::system::error_code;
    using IServiceSession = server_network::IServiceSession;
    using IFileHolder = file_holder::IFileHolder;
    using IStorageDbController = db_controller::IStorageDbController;
    using IExternalServiceChassis = external_service_chassis::IExternalServiceChassis;

public:
    virtual int Run(int port) = 0;
    virtual bool DoOnRead(error_code errocCode, const std::string& request, IServiceSession* serviceSession) = 0;
    virtual void DoOnSignal(const std::string& msg) = 0;
    virtual void DoOnNotified(int eventType, const std::string&, Endpoint from);

    [[nodiscard]] virtual std::string GetFile(const std::string& fileName, const std::string& login) const = 0;
    [[nodiscard]] virtual bool Update(const std::string rootFileName, const std::string& fileName,
                                      const std::string& login, const std::string& fileChanges) const = 0;


    virtual void SetChassis(std::shared_ptr<IExternalServiceChassis> serviceChassis) = 0;
    virtual void SetDbController(std::shared_ptr<IStorageDbController> dbController, int port) = 0;
    virtual void SetFileHolder(std::shared_ptr<IFileHolder> fileHolder) = 0;


    [[nodiscard]] virtual std::string GetAllFilesNames(const std::string& login) const = 0;
    [[nodiscard]] virtual std::string GetGraphArcsForOneVertex(const std::string& rootFileName,
                                                               const std::string& vertexFileName,
                                                               const std::string& login) const = 0;

    virtual void SaveGraphArc(const std::string& login, const std::string& rootFileName,
                              const std::string& fromFileName, const std::string& toFileName) const = 0;
    
    virtual void DeleteFile(const std::string& fileName, const std::string& login) const = 0;

protected:
    virtual bool Create(const std::string& fileName, const std::string& login,
                                      const std::string& rootFileName) const = 0;

protected:
    std::shared_ptr<IFileHolder> m_fileWorker;
    std::shared_ptr<IStorageDbController> m_dbController;
    std::shared_ptr<IExternalServiceChassis> m_serviceChassis;

    std::string m_address{"127.0.0.1"};
    std::uint16_t m_port{};
};
} // namespace inklink:storage

#endif // _BACKEND_SERVICES_STORAGE_INCLUDE_ISTORAGE_H_
