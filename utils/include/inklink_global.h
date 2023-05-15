#pragma once

#include <functional>
#include <string>

namespace inklink
{
struct Endpoint
{
    bool operator==(const Endpoint& rhs) const noexcept
    {
        return (address == rhs.address) && (port == rhs.port);
    }
    std::string address;
    uint16_t port;
};

enum class ServiceType
{
    kServiceRegistry = 0,
    kMessageBroker,
    kApiGateway,
    kAuth,
    kFileStorage,
    kSimultaneousAccess,

    kAll = 100,
    kNone = 101
};

enum class ActionType
{
    kAuth = 0,
    kSelect,
    kDeselect,
    kDrawPolygon,
    kDrawEllipse,
    kDeleteFigure,
    kInsertPixmap,
    kInsertTextBlock,
    kWriteText,
    kDeleteText,
    kFormatText,
    kEndOwnSession,

    kInvalid = 100
};

enum class ResolverActionType
{
    kInsertion = 0,
    kDeletion,
    kFormat,
    kSelect,
    kDeselect
};
} // namespace inklink

// it should not give multiple definitions. The problem is that it is very hard to do it differently
template <>
struct std::hash<inklink::Endpoint>
{
    inline std::size_t operator()(const inklink::Endpoint& endpoint)
    {
        const std::size_t h1 = std::hash<std::string>{}(endpoint.address);
        const std::size_t h2 = std::hash<uint16_t>{}(endpoint.port);
        return h1 ^ (h2 << 1);
    }
};
