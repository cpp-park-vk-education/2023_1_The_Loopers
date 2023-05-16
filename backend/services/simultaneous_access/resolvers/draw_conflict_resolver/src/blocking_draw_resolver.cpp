#include "blocking_draw_resolver.h"

#include "inklink_global.h"

#include <unordered_map>

namespace inklink::service_simultaneous_access
{

std::vector<DrawAction> BlockingDrawResolver::Resolve(std::vector<DrawAction> actions)
{
    std::vector<DrawAction> resolvedActions;

    std::sort(actions.begin(), actions.end(),
              [](const DrawAction& lhs, const DrawAction& rhs) { return lhs.time < rhs.time; });

    std::unordered_map<std::string, Endpoint> userByFigure;

    for (auto& action : actions)
    {
        switch (action.type)
        {
        case ResolverActionType::kSelect:
            if (!userByFigure.contains(action.figureId))
            {
                userByFigure[action.figureId] = action.endpoint;
                resolvedActions.push_back(std::move(action));
            }
            else if (userByFigure[action.figureId] == action.endpoint)
            {
                resolvedActions.push_back(std::move(action));
            }
            break;
        case ResolverActionType::kDeselect:
            if (userByFigure.contains(action.figureId) && userByFigure[action.figureId] == action.endpoint)
            {
                userByFigure.erase(action.figureId);
                resolvedActions.push_back(std::move(action));
            }
            break;
        case ResolverActionType::kDeletion:
            if (userByFigure.contains(action.figureId) && userByFigure[action.figureId] == action.endpoint)
            {
                userByFigure.erase(action.figureId);
                // so that no one in future could select this figure (in this resolving session)
                userByFigure[action.figureId] = {.address = "impossible endpoint", .port = 0};
                resolvedActions.push_back(std::move(action));
            }
            break;
        default:
            if (userByFigure.contains(action.figureId) && userByFigure[action.figureId] == action.endpoint)
            {
                resolvedActions.push_back(std::move(action));
            }
            break;
        }
    }

    return resolvedActions;
}

} // namespace inklink::service_simultaneous_access
