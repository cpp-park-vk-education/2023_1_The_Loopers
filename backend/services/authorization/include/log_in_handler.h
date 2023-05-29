#pragma once

#include "auth_handler.h"

namespace inklink::auth_handler
{
class LogInHandler: public AuthHandler
{
public:
    using HashAndSalt = std::pair<unsigned char*, std::string>;

public:
    bool Handle(const std::string& login, const std::string& password) const override;
};
} // namespace inklink::auth_handler