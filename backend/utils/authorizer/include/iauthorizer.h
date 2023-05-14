#pragma once

#include "itoken_validator.h"

#include <memory>
#include <string>


class IAuthorizer
{
public:
    virtual ~IAuthorizer() = default;

    virtual void SetTokenValidator(ITokenValidator*)
    {
    }

    virtual bool IsAuthenticated(const std::string&)
    {
    }
};
