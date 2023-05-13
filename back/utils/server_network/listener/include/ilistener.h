#ifndef _ILISTENER_H_
#define _ILISTENER_H_

#include <memory>
#include <string>

#include "isessions_factory.h"

class IListener
{
public:
    virtual ~IListener() = default;

    virtual void async_run()
    {
    }
    virtual void SetSessionFactory(ISessionsFactory* factory)
    {
        m_factory = factory;
    }

protected:
    ISessionsFactory* m_factory;
};

#endif  // _ILISTENER_H_
