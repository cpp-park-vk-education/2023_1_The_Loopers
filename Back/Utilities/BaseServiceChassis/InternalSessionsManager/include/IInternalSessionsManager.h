#ifndef _IINTERNALSESSIONSMANAGER_H_
#define _IINTERNALSESSIONSMANAGER_H_

#include <string>
#include <unordered_map>

#include "IServiceSession.h"
#include "global.h"

struct DocSessionDescriptor
{
    std::string documentId;
    std::string login;
};

class IInternalSessionsManager
{
  public:
    struct DocSessionDescriptor
    {
        std::string documentId;
        std::string login;
    };

    virtual ~IInternalSessionsManager() = default;

    virtual void AddSession(DocSessionDescriptor, IServiceSession*)
    {
    }
    virtual void RemoveSession(DocSessionDescriptor)
    {
    }
    virtual vector<ServerSession*> GetSessions(DocSessionDescriptor)
    {
    }
    virtual vector<ServerSession*> GetSessionsByDocument(const std::string&)
    {
    }
    virtual vector<ServerSession*> GetSessionsByUser(const std::string&)
    {
    }
    virtual vector<ServerSession*> GetSessionsByEndpoint(Endpoint)
    {
    }

  protected:
    std::unordered_map<Endpoint, ServerSession*> m_sessions;
    std::unordered_map<std::string, std::string> m_docByUser;
    std::unordered_map<std::string, std::string> m_userByDoc;
    std::unordered_map<DocSessionDescriptor, Endpoint> m_descriptorByEndpoint;
};

#endif  // _IINTERNALSESSIONSMANAGER_H_
