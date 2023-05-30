#pragma once

#include "auth_error.h"
#include "encrypter.h"

#include <iauth_db_controller.h>

#include <memory>

namespace inklink::auth_handler
{
class AuthHandler
{
public:
    using IAuthDbController = inklink::db_controller::IAuthDbController;
    using ITokenGenerator = inklink::token_generator::ITokenGenerator;

public:
    [[nodiscard]] AuthError GetLastError() const;
    void SetLastError(const std::string& errorMsg);

    void SetDbController(const std::shared_ptr<IAuthDbController> dbController);
    void SetTokenGenerator(const std::shared_ptr<ITokenGenerator> tokenGenerator);

    virtual bool handleCredentials(const std::string& login, const std::string& password) const = 0;

    virtual ~AuthHandler() = 0;

protected:
    std::shared_ptr<IAuthDbController> m_dbController;
    std::shared_ptr<ITokenGenerator> m_tokenGenerator; 
    Encrypter m_encrypter;

private:
    AuthError m_lastError;
};
} // namespace inklink::auth_handler
