#include "auth_handler_factory.h"

namespace inklink::auth_handler
{
std::shared_ptr<AuthHandler> AuthHandlerFactory::GetHandler(HandlerType type) const
{
    switch (type)
    {
    case kLogIn:
        return std::make_shared<LogInHandler>();
    case kCreate:
        return std::make_shared<CreateHanlder>();
    }
}
} // namespace inklink::auth_handlers
