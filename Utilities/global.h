#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <string>

struct Endpoint
{
    std::string address;
    unsigned short port;
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

#endif  // _GLOBAL_H_
