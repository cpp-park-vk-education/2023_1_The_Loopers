#pragma once

#include <string>

class Encrypter{
public:
    void setEncryptingKey(std::string key);
    std::string encryptString(const std::string& stringToEncrypt);

private:
    std::string m_encryptingKey;
};
