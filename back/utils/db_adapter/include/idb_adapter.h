#ifndef _BACK_UTILS_DB_ADAPTER_IDBADAPTER_H_
#define _BACK_UTILS_DB_ADAPTER_IDBADAPTER_H_

#include <string>

class IDBAdapter {
  public:
    virtual ~IDBAdapter() = default;

    virtual bool Insert(const std::string& tableName, const std::string& login,
                        const std::string& fileName, const std::string& filePath) = 0;

    virtual bool Update(const std::string& tableName, const std::string& login,
                        const std::string& fileName) = 0;

    virtual std::string Select(const std::string& tableName, const std::string& login,
                               const std::string fileName) = 0;

    virtual bool Delete(const std::string& tableName, const std::string& login,
                        const std::string& fileName) = 0;

    virtual bool Connect() = 0;
    virtual bool Disconnect() = 0;
};

#endif  // _BACK_UTILS_DB_ADAPTER_IDBADAPTER_H_
