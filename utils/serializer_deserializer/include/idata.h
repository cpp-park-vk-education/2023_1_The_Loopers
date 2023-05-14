#pragma once

#include <string>

namespace inklink::serializer
{
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
} // namespace inklink::serializer
