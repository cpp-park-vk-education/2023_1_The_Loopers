#pragma once

#include "iinternal_sessions_manager.h"
#include "ilocal_service_registry.h"
#include "ilogger.h"
#include "imessage_broker_event.h"
#include "imessage_broker_signal.h"

#include <ilistener.h>
#include <memory>
#include <string>
#include <vector>

// #include "iserializer.h"
#include "global.h"
#include "iservice_registator.h"

namespace inklink::base_service_chassis
{
class IBaseServiceChassis
{
public:
    virtual ~IBaseServiceChassis() = default;

    virtual void init(ServiceType, const std::string&, const Endpoint&) = 0;

    // Service register
    virtual bool Register(ServiceType, const Endpoint&) = 0;
    virtual std::vector<Endpoint> GetEndpoints(ServiceType) = 0;

    // local registry
    virtual void AddService(ServiceType, const Endpoint&) = 0;
    virtual std::vector<Endpoint> GetServices(ServiceType) = 0;

    // message broker
    virtual void SetReqResCommonConnection(ICommonConnection*) = 0;
    virtual void Request(const std::string&, const Endpoint&) = 0;
    virtual void SetPubSubCommonConnection(ICommonConnection*) = 0;
    virtual void Publish(int event, const std::string&, ServiceType = ServiceType::kNone) = 0;
    virtual void Subscribe(int event, const Endpoint&) = 0;

    // sessions manager
    virtual void AddSession(DocSessionDescriptor, IServiceSession*) = 0;
    virtual void RemoveSession(DocSessionDescriptor) = 0;

    std::shared_ptr<ILogger> m_logger{std::make_shared<ILogger>()};
    std::shared_ptr<IListener> m_listener{std::make_shared<IListener>()};
    // std::shared_ptr<ISerializer> m_serializer{std::make_shared<ISerializer>()};
    std::shared_ptr<IServiceRegistrator> m_register{std::make_shared<IServiceRegistrator>()};
    std::shared_ptr<IMessageBrokerSignal> m_signalBroker{std::make_shared<IMessageBrokerSignal>()};
    std::shared_ptr<IMessageBrokerEvent> m_eventBroker{std::make_shared<IMessageBrokerEvent>()};
    std::shared_ptr<ILocalServiceRegistry> m_localServiceRegistry{std::make_shared<ILocalServiceRegistry>()};
    std::shared_ptr<IInternalSessionsManager> m_internalSessionsManager{std::make_shared<IInternalSessionsManager>()};

protected:
    ServiceType m_selfType;
    std::string m_name;
    Endpoint m_selfEndpoint;
};
} // namespace inklink::base_service_chassis
