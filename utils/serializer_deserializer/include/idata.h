#pragma once

#include <string>

namespace inklink::serializer
{
class IData
{
public:
    virtual ~IData() = default;

    virtual std::string ToString(std::string) = 0;

    virtual std::string& operator[](std::string) = 0;
};
} // namespace inklink::serializer
