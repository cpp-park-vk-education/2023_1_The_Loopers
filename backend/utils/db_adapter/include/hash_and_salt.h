#pragma once

#include <string>

namespace inklink::auth_handler
{
struct HashAndSalt
{
public:
    HashAndSalt(const std::string& newHash, const std::string newSalt)
    {
        hash = newHash;
        salt = newSalt;
    }
    HashAndSalt() = default;

public:
    std::string hash{};
    std::string salt{};
};
} // namespace inklink::auth_handler
