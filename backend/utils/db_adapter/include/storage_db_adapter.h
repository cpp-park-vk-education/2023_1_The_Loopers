#include "idb_adapter.h"

#include <string>


namespace inklink::db_adapter
{
class StorageDbAdapter : public DbAdapterBase
{
    ~StorageDbAdapter() = default;

    void Connect(const std::string& connectionString) override;
    void Insert(const std::string& request) override;
    void Update(const std::string& request) override;
    void Delete(const std::string& request) override;
    std::string Select(const std::string& request) override;

};
} // namespace inklink::db_adapter
