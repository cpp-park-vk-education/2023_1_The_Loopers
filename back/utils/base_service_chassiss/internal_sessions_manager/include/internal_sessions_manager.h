#ifndef _IINTERNALSESSIONSMANAGER_H_
#define _IINTERNALSESSIONSMANAGER_H_

#include <string>
#include <unordered_map>
#include <vector>

#include "global.h"
#include "iservice_session.h"

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

class InternalSessionsManager
{
public:
    InternalSessionsManager() = default;

    // deleted because injected into sessions and they removes themself at the end of lifetime:
    // it is a lot easier and better to manage lifetime of async session within session itself
    // and it is how it is done in boost::beast examples
    InternalSessionsManager(const InternalSessionsManager&) = delete;
    InternalSessionsManager(InternalSessionsManager&&) = delete;
    InternalSessionsManager& operator=(const InternalSessionsManager&) = delete;

    virtual ~InternalSessionsManager() = default;

    virtual void AddSession(const DocSessionDescriptor&, IServiceSession*);
    virtual void RemoveSession(const DocSessionDescriptor&);
    virtual void RemoveSession(IServiceSession*);

    virtual IServiceSession* GetSession(const DocSessionDescriptor&);
    virtual IServiceSession* GetSession(const Endpoint&);
    virtual std::vector<IServiceSession*> GetSessionsByDocument(const std::string&);
    virtual std::vector<IServiceSession*> GetSessionsByUser(const std::string&);

    virtual DocSessionDescriptor GetDescriptor(const Endpoint&);

protected:
    std::unordered_map<Endpoint, IServiceSession*> m_sessions;
    std::unordered_map<std::string, std::vector<std::string>> m_docsByUser;
    std::unordered_map<std::string, std::vector<std::string>> m_usersByDoc;
    std::unordered_map<DocSessionDescriptor, Endpoint> m_endpointByDescriptor;
    std::unordered_map<Endpoint, DocSessionDescriptor> m_DescriptorByEndpoint;
};

#endif  // _IINTERNALSESSIONSMANAGER_H_
