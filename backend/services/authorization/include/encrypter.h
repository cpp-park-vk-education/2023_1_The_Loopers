#pragma once

#include "hash_and_salt.h"

#include <random>

namespace inklink::auth_handler
{
class Encrypter
{
public:
    [[nodiscard]] HashAndSalt Encrypt(const std::string& password) const;
    [[nodiscard]] std::string EncryptWithSalt(const std::string& password, const std::string& salt) const;

private:
    std::string DynamicSalt(size_t passwordLenght);
};
} // namespace inklink::auth_handler
