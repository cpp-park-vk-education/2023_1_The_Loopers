#ifndef _IAUTHORIZER_H_
#define _IAUTHORIZER_H_

#include <memory>
#include <string>

#include "itoken_validator.h"

class IAuthorizer {
  public:
    virtual ~IAuthorizer() = default;

    virtual void SetTokenValidator(ITokenValidator*) {
    }

    virtual bool IsAuthenticated(const std::string&) {
    }
};

#endif  // _IAUTHORIZER_H_
