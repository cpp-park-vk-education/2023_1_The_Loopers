#pragma once

#include "auth_handler.h"
#include "hash_and_salt.h"

namespace inklink::auth_handler
{
class CreateHandler : public AuthHandler
{
public:
    bool handleCredentials(const std::string& login, const std::string& password) const override;

    ~CreateHandler() = default;

private:
    bool CheckIfUnique(const std::string& login) const;
};
} // namespace inklink::auth_handler
