#pragma once

#include <string>

class IData
{
public:
    virtual ~IData() = default;

    virtual std::string ToString(std::string)
    {
    }

    virtual std::string& operator[](std::string)
    {
    }
};
