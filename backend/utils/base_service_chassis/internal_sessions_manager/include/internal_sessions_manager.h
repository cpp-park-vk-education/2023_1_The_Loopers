#pragma once

#include "inklink_global.h"

#include <string>
#include <unordered_map>
#include <vector>

namespace inklink::server_network
{
class IServiceSession;
} // namespace inklink::server_network

namespace inklink::base_service_chassis
{
struct DocSessionDescriptor
{
    std::string documentId;
    std::string login;
};
} // namespace inklink::base_service_chassis

template <>
struct std::hash<inklink::base_service_chassis::DocSessionDescriptor>
{
    size_t operator()(const inklink::base_service_chassis::DocSessionDescriptor& descriptor)
    {
        const size_t h1 = std::hash<std::string>{}(descriptor.documentId);
        const size_t h2 = std::hash<std::string>{}(descriptor.login);
        return h1 ^ (h2 << 1);
    }
};

namespace inklink::base_service_chassis
{
// can be overloaded, but it is needed only for testing
class InternalSessionsManager
{
    using IServiceSession = inklink::server_network::IServiceSession;

public:
    InternalSessionsManager() = default;

    // deleted because injected into sessions and they removes themself at the end of lifetime:
    // it is a lot easier and better to manage lifetime of async session within session itself
    // and it is how it is done in boost::beast examples
    InternalSessionsManager(const InternalSessionsManager&) = delete;
    InternalSessionsManager(InternalSessionsManager&&) = delete;
    InternalSessionsManager& operator=(const InternalSessionsManager&) = delete;
    InternalSessionsManager& operator=(InternalSessionsManager&&) = delete;

    virtual ~InternalSessionsManager() = default;

    virtual void AddSession(const DocSessionDescriptor&, std::weak_ptr<IServiceSession>);
    virtual void RemoveSession(const DocSessionDescriptor&);
    virtual void RemoveSession(IServiceSession*);
    virtual std::weak_ptr<IServiceSession> GetSession(const DocSessionDescriptor&);
    virtual std::vector<std::weak_ptr<IServiceSession>> GetSessionsByDocument(const std::string&);
    virtual std::vector<std::weak_ptr<IServiceSession>> GetSessionsByUser(const std::string&);
    virtual std::weak_ptr<IServiceSession> GetSession(const Endpoint&);

    virtual DocSessionDescriptor GetDescriptor(const Endpoint&);

protected:
    std::unordered_map<Endpoint, std::weak_ptr<IServiceSession>> m_sessions;
    std::unordered_map<std::string, std::vector<std::string>> m_docsByUser;
    std::unordered_map<std::string, std::vector<std::string>> m_usersByDoc;
    std::unordered_map<DocSessionDescriptor, Endpoint> m_endpointByDescriptor;
    std::unordered_map<Endpoint, DocSessionDescriptor> m_DescriptorByEndpoint;
};
} // namespace inklink::base_service_chassis