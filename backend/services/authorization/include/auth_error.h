#pragma once

#include <string>

namespace inklink::auth_handler
{
struct AuthError
{
public:
    operator bool()
    {
        return !error.empty();
    }

    std::string error;
};
} // namespace inklink::auth_handler
