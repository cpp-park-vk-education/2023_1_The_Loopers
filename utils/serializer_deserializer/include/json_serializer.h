#pragma once

#include "data_container.h"

#include <string>

namespace inklink::serializer
{
class JsonSerializer
{
public:
    static std::string SerializeAsString(const DataContainer&);
    static DataContainer ParseFromString(const std::string&);
};
} // namespace inklink::serializer
