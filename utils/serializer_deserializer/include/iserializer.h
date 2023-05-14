#pragma once

#include "idata.h"

#include <string>

namespace inklink::serializer
{
class ISerializer
{
public:
    virtual ~ISerializer() = default;

    virtual IData* Serialize(const std::string&) = 0;
};
} // namespace inklink::serializer
