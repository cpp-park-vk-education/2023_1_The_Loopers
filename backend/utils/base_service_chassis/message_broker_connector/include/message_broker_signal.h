#pragma once

#include "icommon_connection.h"
#include "inklink_global.h"

#include <boost/system/error_code.hpp>

#include <functional>
#include <memory>
#include <string>

namespace inklink::client_connector
{
class IClientSession;
}

namespace inklink::base_service_chassis
{
class ILogger;

class MessageBrokerSignal
{
private:
    using IClientSession = client_connector::IClientSession;
    using ReadFunctor = std::function<void(const std::string&)>;

public:
    MessageBrokerSignal(std::shared_ptr<ICommonConnection>, ReadFunctor readCallback, std::shared_ptr<ILogger>);
    virtual ~MessageBrokerSignal() = default;

    // virtual void SetDoOnRead(std::function<void(const std::string&, error_code, IClientSession*)>);
    /**
     * @brief request some information from other service
     *
     * @param msgBody serialized body of message to endpoint (no need to add fields needed for msg broker: it will do
     * this class)
     * @param requestFrom endpoint of service to request from
     */
    virtual void Request(const std::string& msgBody, const Endpoint& requestFrom);
    /**
     * @brief send some information to other service
     *
     * @param msgBody serialized body of message to endpoint (no need to add fields needed for msg broker: it will do
     * this class)
     * @param sendTo endpoint of service to send to
     */
    virtual void Send(const std::string& msgBody, const Endpoint& sendTo);

private:
    using error_code = boost::system::error_code;
    void DoOnRead(const std::string&, error_code, IClientSession*) const;

    std::shared_ptr<ICommonConnection> m_connectionToMsgBroker;
    ReadFunctor m_readCallback;
    std::shared_ptr<ILogger> m_logger;
};
} // namespace inklink::base_service_chassis
