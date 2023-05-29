#pragma once

#include "auth_error.h"


namespace inklink::auth_handler
{
class AuthHandler
{
public:
    using IAuthDbController = inklink::db_controller::IAuthDbController;
    using ITokenGenerator = inklink::token_generator::ITokenGenerator;

public:
    AuthError GetLastError() const;
    void SetLastError(const std::string& errorMsg);

    void SetDbController(const IAuthDbController& dbController);
    void SetTokenGenerator(const ITokenGenerator& tokenGenerator);

    virtual bool Handle(const std::string& login, const std::string& password) const = 0;

protected:
    IAuthDbController m_dbController;
    ITokenGenerator m_tokenGenerator; 

private:
    AuthError m_lastError;
};
} // namespace inklink::auth_handler