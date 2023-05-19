#include "internal_sessions_manager.h"

#include <iservice_session.h>

#include <algorithm>
#include <sstream>
#include <stdexcept>

namespace
{
using IServiceSession = inklink::server_network::IServiceSession;
}

namespace inklink::base_service_chassis
{
void InternalSessionsManager::AddSession(const DocSessionDescriptor& descriptor, std::weak_ptr<IServiceSession> session)
{
    const std::shared_ptr<IServiceSession> sessionPtr{session};

    if (descriptor.documentId.empty())
    {
        throw std::logic_error("DocumentId empty");
    }
    if (descriptor.login.empty())
    {
        throw std::logic_error("Login empty");
    }

    const Endpoint& endpoint = sessionPtr->GetClientEndpoint();
    if (m_sessions.contains(endpoint))
    {
        std::ostringstream ss{};
        ss << "Manager already contains enpoint '" << endpoint.address << ':' << endpoint.port << "'";
        throw std::logic_error(ss.str());
    }

    if (m_endpointByDescriptor.contains(descriptor))
    {
        std::ostringstream ss{};
        ss << "User '" << descriptor.login << "' is already using document '" << descriptor.documentId << "'";
        throw std::logic_error(ss.str());
    }

    m_sessions[endpoint] = session;
    m_endpointByDescriptor[descriptor] = endpoint;
    m_DescriptorByEndpoint[endpoint] = descriptor;

    m_docsByUser[descriptor.login].push_back(descriptor.documentId);
    m_usersByDoc[descriptor.documentId].push_back(descriptor.login);
}

void InternalSessionsManager::RemoveSession(const DocSessionDescriptor& descriptor)
{
    if (!m_endpointByDescriptor.contains(descriptor))
    {
        return;
    }

    const auto& endpoint = m_endpointByDescriptor[descriptor];
    m_sessions.erase(endpoint);
    m_DescriptorByEndpoint.erase(endpoint);
    m_endpointByDescriptor.erase(descriptor);

    auto& docs = m_docsByUser[descriptor.login];
    std::erase(docs, descriptor.documentId);

    auto& users = m_usersByDoc[descriptor.documentId];
    std::erase(users, descriptor.login);
}

void InternalSessionsManager::RemoveSession(IServiceSession* session)
{
    if (!session)
    {
        return;
    }

    const Endpoint& endpoint = session->GetClientEndpoint();

    if (!m_DescriptorByEndpoint.contains(endpoint))
    {
        return;
    }
    const DocSessionDescriptor descriptor = m_DescriptorByEndpoint[endpoint];
    RemoveSession(descriptor);
}

std::weak_ptr<IServiceSession>
InternalSessionsManager::GetSession(const DocSessionDescriptor& descriptor) const noexcept
{
    if (!m_endpointByDescriptor.contains(descriptor))
    {
        return {};
    }

    const auto& endpointNode = *m_endpointByDescriptor.find(descriptor);
    const auto& endpoint = endpointNode.second;
    const auto& sessionNode = *m_sessions.find(endpoint);
    return {sessionNode.second};
}

std::weak_ptr<IServiceSession> InternalSessionsManager::GetSession(const Endpoint& endpoint) const noexcept
{
    if (!m_sessions.contains(endpoint))
    {
        return {};
    }

    const auto& sessionNode = *m_sessions.find(endpoint);
    return {sessionNode.second};
}

std::vector<std::weak_ptr<IServiceSession>> InternalSessionsManager::GetSessionsByDocument(const std::string& doc) const
{
    if (!m_usersByDoc.contains(doc))
    {
        return {};
    }

    std::vector<std::weak_ptr<IServiceSession>> sessions{};

    const auto& usersNode = *m_usersByDoc.find(doc);
    for (const auto& user : usersNode.second)
    {
        auto descriptor = DocSessionDescriptor{doc, user};
        sessions.push_back(GetSession(descriptor));
    }

    return sessions;
}

std::vector<std::weak_ptr<IServiceSession>> InternalSessionsManager::GetSessionsByUser(const std::string& login) const
{
    if (!m_docsByUser.contains(login))
    {
        return {};
    }

    std::vector<std::weak_ptr<IServiceSession>> sessions{};

    const auto& docsNode = *m_docsByUser.find(login);
    for (const auto& doc : docsNode.second)
    {
        auto descriptor = DocSessionDescriptor{doc, login};
        sessions.push_back(GetSession(descriptor));
    }

    return sessions;
}

DocSessionDescriptor InternalSessionsManager::GetDescriptor(const Endpoint& endpoint) const noexcept
{
    if (!m_DescriptorByEndpoint.contains(endpoint))
    {
        return {};
    }

    const auto& descriptorNode = *m_DescriptorByEndpoint.find(endpoint);
    return {descriptorNode.second};
}
} // namespace inklink::base_service_chassis
