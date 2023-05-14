#pragma once

#include <memory>
#include <string>

namespace inklink::db_adapter
{
class IDBAdapter
{
public:
    virtual ~IDBAdapter() = default;

    virtual void Insert(const std::string&) = 0;
    virtual void Update(const std::string&) = 0;
    virtual std::string Select(const std::string&) = 0;
    virtual void Delete(const std::string&) = 0;
    virtual void Connect(const std::string&) = 0;
};
} // namespace inklink::db_adapter
