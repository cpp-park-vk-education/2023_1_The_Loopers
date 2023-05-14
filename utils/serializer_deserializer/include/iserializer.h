#pragma once

#include "idata.h"

#include <string>

class ISerializer
{
public:
    virtual ~ISerializer() = default;

    virtual IData* Serialize(std::string)
    {
    }
};
