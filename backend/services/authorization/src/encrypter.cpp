#include "encrypter.h"

#include <openssl/sha.h>

namespace
{
int kAlphabet = 26;
int kMaxSizePasswordWithSalt = 40;
}

namespace inklink::auth_handler
{
Encrypter::HashAndSalt Encrypter::Encrypt(const std::string& password) const
{
    std::string salt = DinamicSault(password.size());

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, (password + salt).c_str(), password.size() + salt.size());

    SHA256_Final(hash, &sha256);

    std::string stringHash{};
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
    {
        stringHash += hash[i];
    }

    return {stringHash, salt};
}
std::string Encrypter::EncryptWithSalt(const std::string& password, const std::string& salt) const
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, (password + salt).c_str(), password.size() + salt.size());

    SHA256_Final(hash, &sha256);

    std::string stringHash{};
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
    {
        stringHash += hash[i];
    }

    return stringHash;
}


std::string Encrypter::DynamicSalt(size_t passwordLenght)
{
    std::string salt{};

    auto saltLength = kMaxSizePasswordWithSalt - passwordLenght;

    srand(time(NULL));

    for (int i = 0; i < saltLength; i++)
    {
        auto ch = 'a' + rand() % kAlphabet;
        salt.push_back(ch);
    }

    return salt;
}
} // namespace inklink::auth_handler
