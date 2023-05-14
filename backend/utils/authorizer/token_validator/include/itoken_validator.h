#pragma once

#include <chrono>
#include <string>

namespace inklink::authorizer
{
class ITokenValidator
{
public:
    virtual ~ITokenValidator() = default;

    virtual std::chrono::time_point<std::chrono::system_clock> ValidateToken(const std::string& token,
                                                                             const std::string& publicKey) = 0;
};
} // namespace inklink::authorizer
