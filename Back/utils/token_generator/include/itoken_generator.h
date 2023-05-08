#ifndef _ITOKENGENERATOR_H_
#define _ITOKENGENERATOR_H_

#include <string>

class ITokenGenerator {
  public:
    virtual ~ITokenGenerator() = default;

    virtual std::string GenerateToken(const std::string& login, const std::string& pass,
                                      const std::string& privateKey) {
    }
};

#endif  // _ITOKENGENERATOR_H_
