#ifndef _ITOKENVALIDATOR_H_
#define _ITOKENVALIDATOR_H_

#include <chrono>
#include <string>

class ITokenValidator {
  public:
    virtual ~ITokenValidator() = default;

    virtual std::chrono::time_point<std::chrono::system_clock> ValidateToken(
            std::string token, std::string publicKey) {
    }
};

#endif  // _ITOKENVALIDATOR_H_
