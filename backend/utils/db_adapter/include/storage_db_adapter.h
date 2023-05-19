#pragma once

#include "idb_adapter.h"
#include "table_after_select.h"

#include <string>


namespace inklink::db_adapter
{
class StorageDbAdapter : public DbAdapterBase final
{
    ~StorageDbAdapter() = default;

    void Connect(const std::string& connectionString) override;
    void Insert(const std::string& request) override;
    void Update(const std::string& request) override;
    void Delete(const std::string& request) override;
    [[nodiscard]] TableAfterSelect Select(const std::string& request) override;

};
} // namespace inklink::db_adapter
