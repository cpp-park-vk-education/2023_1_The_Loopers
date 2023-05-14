#pragma once

#include "global.h"
#include "ibase_service_chassiss.h"

#include <memory>
#include <unordered_map>
#include <vector>


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
