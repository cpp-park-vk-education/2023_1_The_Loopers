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
    inline std::size_t operator()(const inklink::base_service_chassis::DocSessionDescriptor& descriptor)
    {
        const std::size_t h1 = std::hash<std::string>{}(descriptor.documentId);
        const std::size_t h2 = std::hash<std::string>{}(descriptor.login);
        return h1 ^ (h2 << 1);
    }
};

namespace inklink::base_service_chassis
{
class InternalSessionsManager
{
    using IServiceSession = inklink::server_network::IServiceSession;

public:
    virtual ~InternalSessionsManager() = default;

    virtual void AddSession(const DocSessionDescriptor&, std::weak_ptr<IServiceSession>) = 0;
    virtual void RemoveSession(const DocSessionDescriptor&) = 0;
    virtual void RemoveSession(IServiceSession*) = 0;

    virtual std::weak_ptr<IServiceSession> GetSession(const DocSessionDescriptor&) = 0;
    virtual std::vector<std::weak_ptr<IServiceSession>> GetSessionsByDocument(const std::string&) = 0;
    virtual std::vector<std::weak_ptr<IServiceSession>> GetSessionsByUser(const std::string&) = 0;
    virtual std::weak_ptr<IServiceSession> GetSession(const Endpoint&) = 0;

    virtual DocSessionDescriptor GetDescriptor(const Endpoint&) = 0;

protected:
    std::unordered_map<Endpoint, std::weak_ptr<IServiceSession>> m_sessions;
    std::unordered_map<std::string, std::vector<std::string>> m_docByUser;
    std::unordered_map<std::string, std::vector<std::string>> m_userByDoc;
    std::unordered_map<DocSessionDescriptor, Endpoint> m_endpointByDescriptor;
    std::unordered_map<Endpoint, DocSessionDescriptor> m_DescriptorByEndpoint;
};
} // namespace inklink::base_service_chassis
