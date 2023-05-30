#include "auth_handler_factory.h"
#include "create_handler.h"
#include "log_in_handler.h"

namespace inklink::auth_handler
{

std::shared_ptr<AuthHandler> AuthHandlerFactory::GetHandler(HandlerType type) const
{
    switch (type)
    {
    case HandlerType::kLogIn:
        return std::make_shared<LogInHandler>();
    case HandlerType::kCreate:
        return std::make_shared<CreateHandler>();
    }
}
} // namespace inklink::auth_handlers
