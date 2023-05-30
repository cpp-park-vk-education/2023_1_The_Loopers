#pragma once

#include "auth_handler.h"
#include "hash_and_salt.h"

namespace inklink::auth_handler
{
class LogInHandler: public AuthHandler
{
public:
    bool handleCredentials(const std::string& login, const std::string& password) const override;

    ~LogInHandler() = default;
};
} // namespace inklink::auth_handler
