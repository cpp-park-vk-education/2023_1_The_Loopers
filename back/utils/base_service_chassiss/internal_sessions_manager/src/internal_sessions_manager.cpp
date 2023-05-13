#include "internal_sessions_manager.h"

#include <service_session.h>

#include <algorithm
#include <sstream>
#include <stdexcept>

void InternalSessionsManager::AddSession(const DocSessionDescriptor& descriptor,
                                         std::weak_ptr<IServiceSession> session)
{
    std::shared_ptr<IServiceSession> sessionPtr{session};

    const Endpoint& endpoint =->GetClientEndpoint();
    if (!m_sessions.contains(endpoint))
    {
        std::string error{};
        std::ostringstream ss{error};
        ss << "Manager already contains enpoint '" << endpoint.address << ':' << endpoint.port
           << "'";
        throw std::logic_error(error);
    }

    m_sessions[endpoint] = session;
    m_endpointByDescriptor[descriptor] = endpoint;
    m_DescriptorByEndpoint[endpoint] = descriptor;

    m_docsByUser[descriptor.login].push_back(descriptor.documentId);
    m_usersByDoc[descriptor.documentId].push_back(descriptor.login);
}

void InternalSessionsManager::RemoveSession(const DocSessionDescriptor& descriptor)
{
    if (m_endpointByDescriptor.contains(descriptor))
    {
        const Endpoint& endpoint = m_endpointByDescriptor[descriptor];
        m_sessions.erase(endpoint);
        m_DescriptorByEndpoint.erase(endpoint);
        m_endpointByDescriptor.erase(descriptor);

        auto& docs = m_docsByUser[descriptor.login];
        docs.erase(std::remove(docs.begin(), docs.end(), descriptor.documentId), docs.end());

        auto& users = m_usersByDoc[descriptor.documentId];
        users.erase(std::remove(users.begin(), users.end(), descriptor.login), users.end());
    }
}

void InternalSessionsManager::RemoveSession(IServiceSession* session)
{
    const Endpoint& endpoint = session->GetClientEndpoint();

    if (m_DescriptorByEndpoint.contains(endpoint))
    {
        const DocSessionDescriptor& descriptor = m_DescriptorByEndpoint[endpoint];
        m_sessions.erase(endpoint);
        m_endpointByDescriptor.erase(descriptor);
        m_DescriptorByEndpoint.erase(endpoint);

        auto& docs = m_docsByUser[descriptor.login];
        docs.erase(std::remove(docs.begin(), docs.end(), descriptor.documentId), docs.end());

        auto& users = m_usersByDoc[descriptor.documentId];
        users.erase(std::remove(users.begin(), users.end(), descriptor.login), users.end());
    }
}

IServiceSession* InternalSessionsManager::GetSession(const DocSessionDescriptor& descriptor)
{
    IServiceSession* result{nullptr};
    if (m_endpointByDescriptor.contains(descriptor))
    {
        const Endpoint& endpoint = m_endpointByDescriptor[descriptor];
        result = m_sessions[endpoint];
    }
    return result;
}

IServiceSession* InternalSessionsManager::GetSession(const Endpoint& endpoint)
{
    IServiceSession* result{nullptr};
    if (m_sessions.contains(endpoint))
    {
        result = m_sessions[endpoint];
    }
    return result;
}

std::vector<IServiceSession*> InternalSessionsManager::GetSessionsByDocument(const std::string& doc)
{
    std::vector<IServiceSession*> sessions{};
    if (m_usersByDoc.contains(doc))
    {
        for (const std::string& user : m_usersByDoc[doc])
        {
            auto descriptor = DocSessionDescriptor{doc, user};
            sessions.push_back(GetSession(descriptor));
        }
    }
    return sessions;
}

std::vector<IServiceSession*> InternalSessionsManager::GetSessionsByUser(const std::string& login)
{
    std::vector<IServiceSession*> sessions{};
    if (m_docsByUser.contains(login))
    {
        for (const std::string& doc : m_docsByUser[login])
        {
            auto descriptor = DocSessionDescriptor{doc, login};
            sessions.push_back(GetSession(descriptor));
        }
    }
    return sessions;
}

DocSessionDescriptor InternalSessionsManager::GetDescriptor(const Endpoint& endpoint)
{
    DocSessionDescriptor result{};
    if (m_DescriptorByEndpoint.contains(endpoint))
    {
        result = m_DescriptorByEndpoint[endpoint];
    }
    return result;
}
