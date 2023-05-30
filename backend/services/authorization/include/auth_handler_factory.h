#pragma once

#include "auth_handler.h"

#include <memory>

namespace inklink::auth_handler
{
class AuthHandlerFactory
{
    enum class HandlerType
    {
        kLogIn,
        kCreate
    };

public:
    [[nodiscard]] std::shared_ptr<AuthHandler> GetHandler(HandlerType type) const;
};
} // namespace inklink::auth_handler
