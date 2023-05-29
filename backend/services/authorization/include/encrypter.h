#pragma once

#include <openssl/sha.h>

#include <random>

namespace inklink::auth_handler
{
class Encrypter
{
public:
    using HashAndSalt = std::pair<unsigned char*, std::string>;
public:
    [[nodiscard]] HashAndSalt Encrypt(const std::string& password) const;
    [[nodiscard]] unsigned char* EncryptWithSalt(const std::string& password, const std::string& salt) const;

private:
    std::string DinamicSault(int passwordLenght);
};
} // namespace inklink::auth_handler
