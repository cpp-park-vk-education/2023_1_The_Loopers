#pragma once

#include <string>

namespace inklink::token_generator
{
class ITokenGenerator
{
public:
    virtual ~ITokenGenerator() = default;

    virtual std::string GenerateToken(const std::string& login, const std::string& pass, const std::string& privateKey)
    {
    }
};
} // namespace inklink::token_generator
