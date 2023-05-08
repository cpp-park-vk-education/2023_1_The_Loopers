#ifndef _IDATA_H_
#define _IDATA_H_

#include <string>

class IData {
  public:
    virtual ~ISerializer() = default;

    virtual std::string ToString(std::string) {
    }

    virtual std::string& operator[](std::string) {
    }
};

#endif  // _IDATA_H_
