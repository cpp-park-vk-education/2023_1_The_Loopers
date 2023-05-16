#pragma once

#include "ilocal_service_registry.h"
#include "ilogger.h"
#include "imessage_broker_event.h"
#include "imessage_broker_signal.h"
#include "internal_sessions_manager.h"

// #include "iserializer.h"
#include "inklink_global.h"
#include "iservice_registator.h"

#include <ilistener.h>
#include <memory>
#include <string>
#include <vector>

namespace inklink::base_service_chassis
{
class IBaseServiceChassis
{
public:
    virtual ~IBaseServiceChassis() = default;

    virtual void Init(ServiceType, const std::string&, const Endpoint&) = 0;

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
    virtual void Publish(int event, const std::string&, ServiceType) = 0;
    virtual void Subscribe(int event, const Endpoint&) = 0;

    // sessions manager
    virtual void AddSession(DocSessionDescriptor, server_network::IServiceSession*) = 0;
    virtual void RemoveSession(DocSessionDescriptor) = 0;

    std::shared_ptr<ILogger> m_logger;
    std::shared_ptr<server_network::IListener> m_listener;
    // std::shared_ptr<ISerializer> m_serializer{std::make_shared<ISerializer>()};
    std::shared_ptr<IServiceRegistrator> m_register;
    std::shared_ptr<IMessageBrokerSignal> m_signalBroker;
    std::shared_ptr<IMessageBrokerEvent> m_eventBroker;
    std::shared_ptr<ILocalServiceRegistry> m_localServiceRegistry;
    std::shared_ptr<InternalSessionsManager> m_internalSessionsManager;

protected:
    ServiceType m_selfType;
    std::string m_name;
    Endpoint m_selfEndpoint;
};
} // namespace inklink::base_service_chassis
