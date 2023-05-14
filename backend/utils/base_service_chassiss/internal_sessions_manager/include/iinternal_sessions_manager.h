#pragma once

#include "global.h"
#include "iservice_session.h"

#include <string>
#include <unordered_map>
#include <vector>

namespace inklink::base_service_chassis
{
struct DocSessionDescriptor
{
    std::string documentId;
    std::string login;
};

template <>
struct std::hash<DocSessionDescriptor>
{
    size_t operator()(const DocSessionDescriptor& descriptor)
    {
        size_t h1 = std::hash<std::string>{}(descriptor.documentId);
        size_t h2 = std::hash<std::string>{}(descriptor.login);
        return h1 ^ (h2 << 1);
    }
};
class IInternalSessionsManager
{
public:
    virtual ~IInternalSessionsManager() = default;

    virtual void AddSession(const DocSessionDescriptor&, IServiceSession*)
    {
    }
    virtual void RemoveSession(const DocSessionDescriptor&)
    {
    }
    virtual void RemoveSession(IServiceSession*)
    {
    }
    virtual IServiceSession* GetSession(const DocSessionDescriptor&)
    {
    }
    virtual std::vector<IServiceSession*> GetSessionsByDocument(const std::string&)
    {
    }
    virtual std::vector<IServiceSession*> GetSessionsByUser(const std::string&)
    {
    }
    virtual IServiceSession* GetSession(const Endpoint&)
    {
    }

    virtual DocSessionDescriptor GetDescriptor(const Endpoint&)
    {
    }

protected:
    std::unordered_map<Endpoint, IServiceSession*> m_sessions;
    std::unordered_map<std::string, std::vector<std::string>> m_docByUser;
    std::unordered_map<std::string, std::vector<std::string>> m_userByDoc;
    std::unordered_map<DocSessionDescriptor, Endpoint> m_endpointByDescriptor;
    std::unordered_map<Endpoint, DocSessionDescriptor> m_DescriptorByEndpoint;
};
} // namespace inklink::base_service_chassis
