#pragma once

#include <chrono>
#include <string>

namespace inklink::authorizer
{
class ITokenValidator
{
    template <class T>
    using time_point = std::chrono::time_point<T>;
    using system_clock = std::chrono::system_clock;

public:
    virtual ~ITokenValidator() = default;

    virtual time_point<system_clock> ValidateToken(const std::string& token, const std::string& publicKey) = 0;
};
} // namespace inklink::authorizer
