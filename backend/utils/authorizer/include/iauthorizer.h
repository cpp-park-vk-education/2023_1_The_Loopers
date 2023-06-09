#pragma once

#include "itoken_validator.h"

#include <memory>
#include <string>

namespace inklink::authorizer
{
class IAuthorizer
{
public:
    explicit IAuthorizer(std::shared_ptr<ITokenValidator> v) noexcept : m_validator{std::move(v)}
    {
    }
    virtual ~IAuthorizer() = default;

    virtual bool IsAuthenticated(const std::string&) = 0;

protected:
    std::shared_ptr<ITokenValidator> m_validator;
};
} // namespace inklink::authorizer
