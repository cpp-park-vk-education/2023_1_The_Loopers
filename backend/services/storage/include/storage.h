#ifndef _BACKEND_SERVICES_STORAGE_INCLUDE_STORAGE_H_
#define _BACKEND_SERVICES_STORAGE_INCLUDE_STORAGE_H_

#include "istorage.h"

namespace inklink::storage
{
class Storage final : public IStorage
{
public:
    using IFileHolder = file_holder::IFileHolder;
    using IStorageDbController = db_controller::IStorageDbController;
    using IExternalServiceChassis = external_service_chassis::IExternalServiceChassis;

public:
    int Run(int port) override;

    bool DoOnRead(error_code errocCode, const std::string& request, IServiceSession* serviceSession) override;
    void DoOnSignal(const std::string& msg) override;
    void DoOnNotified(int eventType, const std::string&, Endpoint from) override;

    [[nodiscard]] std::string GetFile(const std::string& fileName, const std::string& login) const override;
    [[nodiscard]] bool Update(const std::string rootFileName, const std::string& fileName, const std::string& login,
                              const std::string& fileChanges) const override;

    void SetChassis(std::shared_ptr<IExternalServiceChassis> serviceChassis)  override;
    void SetDbController(std::shared_ptr<IStorageDbController> dbController, int port) override;
    void SetFileHolder(std::shared_ptr<IFileHolder> fileHolder) override;


    [[nodiscard]] std::string GetAllFilesNames(const std::string& login) const override;
    [[nodiscard]] std::string GetGraphArcsForOneVertex(const std::string& rootFileName, const std::string& vertexFileName,
                                                       const std::string& login) const override;
    void SaveGraphArc(const std::string& rootFileName, const std::string& fromFileName, const std::string& toFileName) const override;

    void DeleteFile(const std::string& fileName, const std::string& login) const override;

private:
    [[nodiscard]] bool Create(const std::string& fileName, const std::string& login,
                              const std::string& rootFileName) const override;
};
} // namespace inklink:storage

#endif // _BACKEND_SERVICES_STORAGE_INCLUDE_STORAGE_H_
