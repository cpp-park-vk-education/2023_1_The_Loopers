#include "blocking_draw_resolver.h"

#include "inklink_global.h"

namespace inklink::service_simultaneous_access
{

std::vector<DrawAction> BlockingDrawResolver::Resolve(std::vector<DrawAction> actions)
{
    std::vector<DrawAction> resolvedActions;

    std::sort(actions.begin(), actions.end(),
              [](const DrawAction& lhs, const DrawAction& rhs) { return lhs.time < rhs.time; });

    for (auto& action : actions)
    {
        switch (action.type)
        {
        case ResolverActionType::kSelect:
            if (!m_userBySelectedFigure.contains(action.figureId))
            {
                m_userBySelectedFigure[action.figureId] = action.endpoint;
                resolvedActions.push_back(std::move(action));
            }
            else if (m_userBySelectedFigure[action.figureId] == action.endpoint)
            {
                resolvedActions.push_back(std::move(action));
            }
            break;
        case ResolverActionType::kDeselect:
            if (m_userBySelectedFigure.contains(action.figureId) &&
                m_userBySelectedFigure[action.figureId] == action.endpoint)
            {
                m_userBySelectedFigure.erase(action.figureId);
                resolvedActions.push_back(std::move(action));
            }
            break;
        case ResolverActionType::kDeletion:
            if (m_userBySelectedFigure.contains(action.figureId) &&
                m_userBySelectedFigure[action.figureId] == action.endpoint)
            {
                m_userBySelectedFigure.erase(action.figureId);
                // so that no one in future could select this figure (in this resolving session)
                m_userBySelectedFigure[action.figureId] = {.address = "impossible endpoint", .port = 0};
                resolvedActions.push_back(std::move(action));
            }
            break;
        default:
            if (m_userBySelectedFigure.contains(action.figureId) &&
                m_userBySelectedFigure[action.figureId] == action.endpoint)
            {
                resolvedActions.push_back(std::move(action));
            }
            break;
        }
    }

    return resolvedActions;
}

} // namespace inklink::service_simultaneous_access
