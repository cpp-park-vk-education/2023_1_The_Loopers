#ifndef _ISERIALIZER_H_
#define _ISERIALIZER_H_

#include <string>

#include "IData.h"

class ISerializer {
  public:
    virtual ~ISerializer() = default;

    virtual IData* Serialize(std::string) {
    }
};

#endif  // _ISERIALIZER_H_
