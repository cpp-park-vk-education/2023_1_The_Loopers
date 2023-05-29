#include "encrypter.h"

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

    return {hash, salt};
}
unsigned char* Encrypter::EncryptWithSalt(const std::string& password, const std::string& salt) const
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, (password + salt).c_str(), password.size() + salt.size());

    SHA256_Final(hash, &sha256);

    return hash;
}


std::string Encrypter::DinamicSault(int passwordLenght)
{
    std::string salt{};

    auto saltLength = 20 - passwordLenght;

    srand(time(NULL));

    for (int i = 0; i < saltLength; i++)
    {
        auto ch = 'a' + rand() % 26;
        salt.push_back(ch);
    }

    return salt;
}
} // namespace inklink::auth_handler
