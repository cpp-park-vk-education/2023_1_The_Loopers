#ifndef _IBASESERVICECHASSIS_H_
#define _IBASESERVICECHASSIS_H_

#include <memory>
#include <string>
#include <vector>

#include "IInternalSessionsManager.h"
#include "ILocalServiceRegistry.h"
#include "ILogger.h"
#include "IMessageBrokerEvent.h"
#include "IMessageBrokerSignal.h"
#include "ISerializer.h"
#include "IServiceRegistrator.h"

class IBaseServiceChassis {
  public:
    virtual ~IBaseServiceChassis() = default;

    virtual void LogDebug(const std::string&) {
    }

    virtual void init(ServiceType, name : string, Endpoint) {
    }

    // Service register
    virtual bool register(ServiceType, Endpoint) {
    }
    virtual std::vector<Endpoint> GetEndpoints(ServiceType) {
    }
    // local registry
    virtual void AddService(ServiceType, Endpoint) {
    }
    virtual std::vector<Endpoint> GetServices(ServiceType) {
    }

    // message broker
    virtual void SetReqResCommonConnection(CommonConnection*) {
    }
    virtual void Request(body : string, Endpoint) {
    }
    virtual void SetPubSubCommonConnection(CommonConnection*) {
    }
    virtual void Publish(event : int, body : string, ServiceType = None) {
    }
    virtual void Subscribe(event : int, self : Endpoint) {
    }

    // sessions manager
    virtual void AddSession(SessionDescriptor, Session*) {
    }
    virtual void RemoveSession(SessionDescriptor) {
    }

    std::shared_ptr<ILoggerAdapter> m_logger;
    std::shared_ptr<IListener> m_listener;
    std::shared_ptr<ISerializer> m_serializer;
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

#endif  // _IBASESERVICECHASSIS_H_

IBaseServiceChassis