#pragma once

#include "auth_handler.h"

#include <memory>

namespace inklink::auth_handler
{
enum class HandlerType
{
    kLogIn,
    kCreate
};

class AuthHandlerFactory
{
public:
    [[nodiscard]] std::shared_ptr<AuthHandler> GetHandler(HandlerType type) const;
};
} // namespace inklink::auth_handler
