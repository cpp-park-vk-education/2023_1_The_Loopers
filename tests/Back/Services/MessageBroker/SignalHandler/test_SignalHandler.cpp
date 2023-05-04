#ifndef _ISIGNALSHANDLER_H_
#define _ISIGNALSHANDLER_H_

#include <memory>
#include <unordered_map>
#include <vector>

#include "IBaseServiceChassis.h"
#include "global.h"

class ISignalsHandler
{
  public:
    virtual ~ISignalsHandler() = default;

    virtual void SetServiceChassis(BaseServiceChassis*)
    {
    }

    virtual void SendMessage(Endpoint, const std::string&)
    {
    }

  protected:
    std::shared_ptr<BaseServiceChassis> m_serviceChassis;
};

#endif  // _ISIGNALSHANDLER_H_
