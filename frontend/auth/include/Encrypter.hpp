#pragma once

#include <string>

namespace inklink::auth
{
class Encrypter
{
public:
    void setEncryptingKey(const std::string& key);
    std::string encryptString(const std::string& stringToEncrypt);

private:
    std::string m_encryptingKey;
};
} // namespace inklink::auth
