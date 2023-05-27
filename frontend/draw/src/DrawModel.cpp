#include "DrawModel.h"
#include "DrawView.h"
#include "data_container.h"
#include "json_serializer.h"
#include "websocket_client_session.h" // Sasha Novak says it should be in <> scopes, but i don't really know

namespace inklink::draw
{
std::string DrawModel::SerializeToSend(int actionId, int actionType, int figureId)
{
    DataContainer sendContainer{};
    sendContainer["document_id"] = m_filename;
    sendContainer["action"]["action_id"] = actionId;
    sendContainer["action"]["action_type"] = actionType;
    sendContainer["time"] = "now";
    sendContainer["figure_id"] = figureId;
    return JsonSerializer::SerializeAsString(sendContainer);
}

void DrawModel::ParseToGet(const std::string& message) {
    DataContainer gotData = JsonSerializer::ParseFromString(message);
    m_view->NotifyGotResultFromNetwork(true);
    return;
}
} // namespace inklink::draw
