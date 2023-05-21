#include "blocking_draw_resolver.h"

#include "inklink_global.h"

namespace inklink::service_simultaneous_access
{
BlockingDrawResolver::BlockingDrawResolver(std::size_t maxFigureId) noexcept
        : IDrawConflictResolver{}, m_maxFigureId{maxFigureId}
{
}

void BlockingDrawResolver::Resolve(std::vector<DrawAction>& actions)
{
    std::vector<DrawAction> resolvedActions;

    std::sort(actions.begin(), actions.end(),
              [](const DrawAction& lhs, const DrawAction& rhs) { return lhs.time < rhs.time; });

    for (auto& action : actions)
    {
        switch (action.type)
        {
        case ResolverActionType::kInsertion:
            action.figureId = std::to_string(++m_maxFigureId);
            m_userBySelectedFigure[action.figureId] = action.endpoint;
            resolvedActions.push_back(std::move(action));
            m_history.push_back(action);
            break;
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
                m_history.push_back(action);
            }
            break;
        default:
            if (m_userBySelectedFigure.contains(action.figureId) &&
                m_userBySelectedFigure[action.figureId] == action.endpoint)
            {
                resolvedActions.push_back(std::move(action));
                m_history.push_back(action);
            }
            break;
        }
    }

    actions = std::move(resolvedActions);
}

} // namespace inklink::service_simultaneous_access
