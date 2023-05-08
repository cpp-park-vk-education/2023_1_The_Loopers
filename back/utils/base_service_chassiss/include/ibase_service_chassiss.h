#ifndef _IBASESERVICECHASSISS_H_
#define _IBASESERVICECHASSISS_H_

#include <ilistener.h>

#include <memory>
#include <string>
#include <vector>

#include "iinternal_sessions_manager.h"
#include "ilocal_service_registry.h"
#include "ilogger.h"
#include "imessage_broker_event.h"
#include "imessage_broker_signal.h"
// #include "iserializer.h"
#include "global.h"
#include "iservice_registator.h"

class IBaseServiceChassis
{
public:
    virtual ~IBaseServiceChassis() = default;

    virtual void init(ServiceType, const std::string&, const Endpoint&)
    {
    }

    // Service register
    virtual bool Register(ServiceType, const Endpoint&)
    {
    }
    virtual std::vector<Endpoint> GetEndpoints(ServiceType)
    {
    }

    // local registry
    virtual void AddService(ServiceType, const Endpoint&)
    {
    }
    virtual std::vector<Endpoint> GetServices(ServiceType)
    {
    }

    // message broker
    virtual void SetReqResCommonConnection(ICommonConnection*)
    {
    }
    virtual void Request(const std::string&, const Endpoint&)
    {
    }
    virtual void SetPubSubCommonConnection(ICommonConnection*)
    {
    }
    virtual void Publish(int event, const std::string&, ServiceType = ServiceType::kNone)
    {
    }
    virtual void Subscribe(int event, const Endpoint&)
    {
    }

    // sessions manager
    virtual void AddSession(DocSessionDescriptor, IServiceSession*)
    {
    }
    virtual void RemoveSession(DocSessionDescriptor)
    {
    }

    std::shared_ptr<ILogger> m_logger{std::make_shared<ILogger>()};
    std::shared_ptr<IListener> m_listener{std::make_shared<IListener>()};
    // std::shared_ptr<ISerializer> m_serializer{std::make_shared<ISerializer>()};
    std::shared_ptr<IServiceRegistrator> m_register{std::make_shared<IServiceRegistrator>()};
    std::shared_ptr<IMessageBrokerSignal> m_signalBroker{std::make_shared<IMessageBrokerSignal>()};
    std::shared_ptr<IMessageBrokerEvent> m_eventBroker{std::make_shared<IMessageBrokerEvent>()};
    std::shared_ptr<ILocalServiceRegistry> m_localServiceRegistry{
            std::make_shared<ILocalServiceRegistry>()};
    std::shared_ptr<IInternalSessionsManager> m_internalSessionsManager{
            std::make_shared<IInternalSessionsManager>()};

protected:
    ServiceType m_selfType;
    std::string m_name;
    Endpoint m_selfEndpoint;
};

#endif  // _IBASESERVICECHASSISS_H_
