#pragma once

#include <openssl/sha.h>

namespace inklink::auth_handler
{
class Encrypter
{
public:
    [[nodiscard]] std::string Encrypt(const std::string& password) const;
    [[nodiscard]] std::string Decrypt(const std::string& hash) const;

private:
    std::string DinamicSault(int passwordLenght);
};
} // namespace inklink::auth_handler
