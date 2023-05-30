#include "isignals_handler.h"

#include <ibase_service_chassis.h>

#include <json_serializer.h>

namespace
{
using IBaseServiceChassis = inklink::base_service_chassis::IBaseServiceChassis;
using JsonSerializer = inklink::serializer::JsonSerializer;
} // namespace

namespace inklink::service_message_broker
{
ISignalsHandler::ISignalsHandler(IBaseServiceChassis& chassis) : m_serviceChassis{chassis}
{
}

bool ISignalsHandler::Handle(const DataContainer& msgData, const Endpoint& sender)
{
    if (msgData.Has("receiver") && msgData.Has("message_body"))
    {
        if (!msgData["receiver"].Has("address") || !msgData["receiver"].Has("port"))
        {
            return false;
        }
        const Endpoint endpointTo{.address = msgData["receiver"].AsString("address"),
                                  .port = static_cast<std::uint16_t>(msgData["receiver"].AsInt("port"))};
        DataContainer newMsg{};
        newMsg["sender"]["address"] = sender.address;
        newMsg["sender"]["port"] = static_cast<int>(sender.port);
        newMsg["message_body"] = msgData["message_body"];
        newMsg["time"] = std::string("now");
        Send(endpointTo, JsonSerializer::SerializeAsString(newMsg));
        return true;
    }
}

void ISignalsHandler::Send(const Endpoint& endpoint, const std::string& msg)
{
    auto session = m_serviceChassis.internalSessionsManager->GetSession(endpoint);
    if (session.expired())
    {
        return;
    }
    auto sessionLocked = session.lock();
    if (sessionLocked)
    {
        sessionLocked->Send(msg);
    }
}

} // namespace inklink::service_message_broker
