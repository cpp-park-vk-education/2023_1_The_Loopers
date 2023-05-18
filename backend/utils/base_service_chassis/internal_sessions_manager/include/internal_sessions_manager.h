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

    bool operator==(const DocSessionDescriptor& rhs) const noexcept
    {
        return documentId == rhs.documentId && login == rhs.login;
    }
};
} // namespace inklink::base_service_chassis

template <>
struct std::hash<inklink::base_service_chassis::DocSessionDescriptor>
{
    inline std::size_t operator()(const inklink::base_service_chassis::DocSessionDescriptor& descriptor) const
    {
        const std::size_t h1 = std::hash<std::string>{}(descriptor.documentId);
        const std::size_t h2 = std::hash<std::string>{}(descriptor.login);
        return h1 ^ (h2 << 1);
    }
};

namespace inklink::base_service_chassis
{
// virtual for testing
class InternalSessionsManager
{
    using IServiceSession = inklink::server_network::IServiceSession;

public:
    InternalSessionsManager(const InternalSessionsManager&) = delete;
    InternalSessionsManager(InternalSessionsManager&&) = delete;
    InternalSessionsManager& operator=(const InternalSessionsManager&) = delete;
    InternalSessionsManager& operator=(InternalSessionsManager&&) = delete;

    virtual ~InternalSessionsManager() = default;

    virtual void AddSession(const DocSessionDescriptor&, std::weak_ptr<IServiceSession>);
    virtual void RemoveSession(const DocSessionDescriptor&);
    virtual void RemoveSession(IServiceSession*);

    [[nodiscard]] virtual std::weak_ptr<IServiceSession> GetSession(const DocSessionDescriptor&) const noexcept;
    [[nodiscard]] virtual std::weak_ptr<IServiceSession> GetSession(const Endpoint&) const noexcept;

    [[nodiscard]] virtual std::vector<std::weak_ptr<IServiceSession>>
    GetSessionsByDocument(const std::string& documentId) const;
    [[nodiscard]] virtual std::vector<std::weak_ptr<IServiceSession>>
    GetSessionsByUser(const std::string& userLogin) const;

    [[nodiscard]] virtual DocSessionDescriptor GetDescriptor(const Endpoint&) const noexcept;

protected:
    std::unordered_map<Endpoint, std::weak_ptr<IServiceSession>> m_sessions;
    std::unordered_map<std::string, std::vector<std::string>> m_docsByUser;
    std::unordered_map<std::string, std::vector<std::string>> m_usersByDoc;
    // TODO (a.novak) now user can connect only from one device
    std::unordered_map<DocSessionDescriptor, Endpoint> m_endpointByDescriptor;
    std::unordered_map<Endpoint, DocSessionDescriptor> m_DescriptorByEndpoint;
};
} // namespace inklink::base_service_chassis
