#pragma once

#include "itoken_validator.h"

#include <memory>
#include <string>

namespace inklink::authorizer
{
class IAuthorizer
{
public:
    virtual ~IAuthorizer() = default;

    virtual void SetTokenValidator(ITokenValidator*) = 0;

    virtual bool IsAuthenticated(const std::string&) = 0;
};
} // namespace inklink::authorizer
