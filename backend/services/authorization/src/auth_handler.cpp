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

void AuthHandler::SetDbController(const IAuthDbController& dbController)
{
    m_dbController = dbController;
}
void AuthHandler::SetTokenGenerator(const ITokenGenerator& tokenGenerator) 
{
    m_tokenGenerator = tokenGenerator;
}
} // namespace inklink::auth_handler