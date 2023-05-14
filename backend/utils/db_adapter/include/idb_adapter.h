#pragma once

#include <memory>
#include <string>

class IDBAdapter
{
public:
    virtual ~IDBAdapter() = default;

    virtual void Insert(const std::string&)
    {
    }
    virtual void Update(const std::string&)
    {
    }
    virtual std::string Select(const std::string&)
    {
    }
    virtual void Delete(const std::string&)
    {
    }
    virtual void Connect(const std::string&)
    {
    }
};
