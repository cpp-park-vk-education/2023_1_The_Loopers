#include "isignals_handler.h"

#include <ibase_service_chassis.h>

#include <json_serializer.h>

namespace
{
using IBaseServiceChassis = inklink::base_service_chassis::IBaseServiceChassis;
} // namespace

namespace inklink::service_message_broker
{
ISignalsHandler::ISignalsHandler(std::unique_ptr<IBaseServiceChassis>& chassis) : m_serviceChassis{chassis}
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
        const Endpoint enpointTo{.address = msgData["receiver"].AsString("address"),
                                 .port = static_cast<std::uint16_t>(msgData["receiver"].AsInt("port"))};
        DataContainer newMsg{};
        newMsg["sender"]["address"] = sender.address;
        newMsg["sender"]["port"] = static_cast<int>(sender.port);
        newMsg["message_body"] = msgData["message_body"];
        newMsg["time"] = "now";
        SendEvent(enpointTo, JsonParser::SerializeAsString(newMsg));
        return true;
    }
}

void ISignalsHandler::Send(const Endpoint& endpoint, const std::string& msg)
{
    auto session = chassis->manager->GetSession(subscriber).lock();
    if (session)
    {
        session->Send(msg);
    }
}

} // namespace inklink::service_message_broker
