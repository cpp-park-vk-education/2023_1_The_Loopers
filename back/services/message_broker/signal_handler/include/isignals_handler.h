#ifndef _ISIGNALSHANDLER_H_
#define _ISIGNALSHANDLER_H_

#include <memory>
#include <unordered_map>
#include <vector>

#include "global.h"
#include "ibase_service_chassiss.h"

class ISignalsHandler
{
public:
    virtual ~ISignalsHandler() = default;

    virtual void SetServiceChassis(std::shared_ptr<IBaseServiceChassis>)
    {
    }

    virtual void Send(Endpoint, const std::string&)
    {
    }

protected:
    std::shared_ptr<IBaseServiceChassis> m_serviceChassis;
};

#endif  // _ISIGNALSHANDLER_H_
