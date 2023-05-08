#ifndef _IEVENTSHANDLER_H_
#define _IEVENTSHANDLER_H_

#include <memory>
#include <unordered_map>
#include <vector>

#include "global.h"
#include "ibase_service_chassiss.h"

class IEventsHandler
{
public:
    virtual ~IEventsHandler() = default;

    virtual void SetServiceChassis(std::shared_ptr<IBaseServiceChassis>)
    {
    }

    virtual void RemoveSubscriber(int, Endpoint)
    {
    }
    virtual void AddSubscriber(int, Endpoint)
    {
    }
    virtual void SendEvent(int, std::string)
    {
    }

protected:
    std::shared_ptr<IBaseServiceChassis> m_serviceChassis{std::make_shared<IBaseServiceChassis>()};

    std::unordered_map<int, std::vector<Endpoint>> m_subscribers;
};

#endif  // _IEVENTSHANDLER_H_