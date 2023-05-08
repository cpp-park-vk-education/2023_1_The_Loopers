#ifndef _IINTERNALSESSIONSMANAGER_H_
#define _IINTERNALSESSIONSMANAGER_H_

#include <string>
#include <unordered_map>
#include <vector>

#include "iservice_session.h"
#include "global.h"

class IInternalSessionsManager
{
  public:
    struct DocSessionDescriptor
    {
        std::string documentId;
        std::string login;
    };

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
    virtual IServiceSession* GetSession(Endpoint)
    {
    }

  protected:
    std::unordered_map<Endpoint, IServiceSession*> m_sessions;
    std::unordered_map<std::string, std::string> m_docByUser;
    std::unordered_map<std::string, std::string> m_userByDoc;
    std::unordered_map<DocSessionDescriptor, Endpoint> m_endpointByDescriptor;
};

#endif  // _IINTERNALSESSIONSMANAGER_H_
