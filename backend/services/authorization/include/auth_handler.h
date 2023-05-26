#pragma once

#include "iauth_handler.h"

namespace inklink::auth_handler
{
class AuthHandler
{
public:
    using IAuthDbController = inklink::db_controller::IAuthDbController;
    using ITokenGenerator = inklink::token_generator::ITokenGenerator;

public:
    void SetDbController(const IAuthDbController& dbController) override;
    void SetTokenGenerator(const ITokenGenerator& tokenGenerator) override;
    virtual bool Handle(const std::string& login, const std::string& password) const = 0;

protected:
    IAuthDbController m_dbController;
    ITokenGenerator m_tokenGenerator;
};
} // namespace inklink::auth_handler