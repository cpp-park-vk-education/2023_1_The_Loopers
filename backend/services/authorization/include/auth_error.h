#pragma once

#include <string>


namespace inklink::auth_handler
{
class AuthError
{
public:
    operator bool()
    {
        return !error.empty();
    }

    std::string error;
};
} // namespace inklink::auth_handler
