#pragma once

#include <chrono>
#include <string>

namespace inklink::authorizer
{
class ITokenValidator
{
public:
    virtual ~ITokenValidator() = default;

    virtual std::chrono::time_point<std::chrono::system_clock> ValidateToken(std::string token, std::string publicKey)
    {
    }
};
Ъ
