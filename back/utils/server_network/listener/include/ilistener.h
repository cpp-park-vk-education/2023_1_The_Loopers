#ifndef _ILISTENER_H_
#define _ILISTENER_H_

#include <memory>
#include <string>

#include "isessions_factory.h"

namespace inklink_listener
{
class IListener
{
public:
    using ISessionsFactory = inklink_sessions_factory::ISessionsFactory;

    virtual ~IListener() = default;

    virtual void async_run()
    {
    }
    virtual void SetSessionFactory(std::shared_ptr<ISessionsFactory> factory)
    {
        m_factory = factory;
    }

protected:
    std::shared_ptr<ISessionsFactory> m_factory;
};
}  // namespace inklink_listener

#endif  // _ILISTENER_H_
