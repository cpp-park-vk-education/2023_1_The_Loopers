#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <memory>
#include <string>

struct Endpoint {
    std::string address;
    unsigned short port;
};

enum ServiceType {
    kServiceRegistry = 0,
    kMessageBroker,
    kApiGateway,
    kAuth,
    kFileStorage,
    kSimultaneousAccess,

    kAll = 100,
    kNone = 101
};

enum ActionType {
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
    kEndOwnSession
}

#endif  // _GLOBAL_H_
