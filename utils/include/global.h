#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <functional>
#include <string>

struct Endpoint
{
    bool operator==(const Endpoint& rhs) const noexcept
    {
        return (address == rhs.address) && (port == rhs.port);
    }
    std::string address;
    unsigned short port;
};

template <>
struct std::hash<Endpoint>
{
    size_t operator()(const Endpoint& endpoint)
    {
        size_t h1 = std::hash<std::string>{}(endpoint.address);
        size_t h2 = std::hash<unsigned short>{}(endpoint.port);
        return h1 ^ (h2 << 1);
    }
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

// template<>
// struct std::hash<ServiceType> {
//     size_t operator()(const ServiceType&) {

//     }
// };

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

#endif  // _GLOBAL_H_
