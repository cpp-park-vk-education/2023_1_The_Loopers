
#include <memory>
#include <string>

#include "iauthorizer.h"

class IAuthorizer
{
  public:
    virtual ~IAuthorizer() = default;

    virtual void SetTokenValidator(ITokenValidator*)
    {
    }

    virtual bool IsAuthenticated(const std::string&)
    {
    }
};
