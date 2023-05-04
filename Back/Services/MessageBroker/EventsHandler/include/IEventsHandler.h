#ifndef _IEVENTSHANDLER_H_
#define _IEVENTSHANDLER_H_

#include <memory>
#include <unordered_map>
#include <vector>

#include "IBaseServiceChassis.h"
#include "global.h"

class IEventsHandler
{
  public:
    virtual ~IEventsHandler() = default;

    virtual void SetServiceChassis(BaseServiceChassis*)
    {
    }

    virtual void AddSubscriber(int, Endpoint)
    {
    }
    virtual void SendEvent(int, const std::string&)
    {
    }

  protected:
    std::shared_ptr<BaseServiceChassis> m_serviceChassis;

    std::unordered_map<int, vector<Endpoint>> m_subscribers;
};

#endif  // _IEVENTSHANDLER_H_
