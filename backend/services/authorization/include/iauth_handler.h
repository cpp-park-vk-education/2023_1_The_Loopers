#pragma once

#include "istorage_db_controller.h"
#include "itoken_generator.h"

#include <string>

namespace inklink::auth_handler
{
class IAuthHandler
{
public:
    using IAuthDbController = inklink::db_controller::IAuthDbController;
    using ITokenGenerator = inklink::token_generator::ITokenGenerator;

public:
    virtual void SetDbController(const IAuthDbController& dbController) = 0;
    virtual void SetTokenGenerator(const ITokenGenerator& tokenGenerator) = 0;
    virtual bool Handle(const std::string& login, const std::string& password) = 0;

protected: 
    IAuthDbController m_dbController;
    ITokenGenerator m_tokenGenerator;
};
}