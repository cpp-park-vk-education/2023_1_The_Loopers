#ifndef _IINTERNALSESSIONSMANAGER_H_
#define _IINTERNALSESSIONSMANAGER_H_

#include <string>

#include "IServerSession.h"
#include "global.h"

class IInternalSessionsManager {
  public:
    struct DocSessionDescriptor {
        std::string documentId;
        std::string login;
    };

    virtual ~IInternalSessionsManager() = default;

    virtual void AddSession(DocSessionDescriptor, IServerSession*) {
    }
    virtual void RemoveSession(DocSessionDescriptor) {
    }
    virtual vector<Session*> GetSessions(DocSessionDescriptor) {
    }
    virtual vector<Session*> GetSessionsByDocument(const std::string&) {
    }
    virtual vector<Session*> GetSessionsByUser(const std::string&) {
    }
    virtual vector<Session*> GetSessionsByEndpoint(Endpoint) {
    }
};

#endif  // _IINTERNALSESSIONSMANAGER_H_
