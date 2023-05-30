#include "auth_handler.h"


namespace inklink::auth_handler
{
void AuthHandler::SetLastError(const std::string& errorMsg)
{
    m_lastError.error = errorMsg;
}

AuthError AuthHandler::GetLastError() const
{
    return m_lastError;
}

void AuthHandler::SetDbController(const std::shared_ptr<IAuthDbController> dbController)
{
    m_dbController = dbController;
}

void AuthHandler::SetTokenGenerator(const std::shared_ptr<ITokenGenerator> tokenGenerator)
{
    m_tokenGenerator = tokenGenerator;
}
} // namespace inklink::auth_handler
