#pragma once

#include "IObject.h"

#include <QGraphicsScene>

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

#include <string>
#include <thread>
#include <vector>

namespace inklink::client_connector
{
class IClientSession;
} // namespace inklink::client_connector

namespace inklink::draw
{
class DrawingView;
} // namespace inklink::draw

using namespace boost::asio;

namespace inklink::draw
{
class DrawSceneModel : public QGraphicsScene
{
    Q_OBJECT

public:
    DrawSceneModel(QObject* parent = nullptr);

    ~DrawSceneModel();

    std::string Serialize(int actionType, int figureId, int type);
    void Send(std::string& message);
    void SetFilename(std::string& filename);

    // protected:
    //     void addObject(size_t, size_t, std::vector<Point>&);
    //     void addObject(size_t, Point&, size_t, size_t);
    //     void addObject(size_t, std::string&, Point&, size_t);

signals:
    // void NewObject(IObject*); // QObject->setParent(DrawView);
    // or in model when constructing IObject(DrawView);

    void GotNewMsg(const char* msg);
    // DoOnRead(const std::string& msg) { emit GotNewMsg(msg.c_str()); }
    // so that it will be called in "main" thread

    //
    // Q_DECLARE_META_TYPE(DataContainer);
    // qRegisterMetaType(DataContainer); // if you want to use DataContainer in signals/slots
    //

private:
    using IClientSession = client_connector::IClientSession;

private:
    void Deserialize(const std::string& message);
    // [[nodiscard]] int GenerateRandomNumber();

    DrawingView* m_view = nullptr;
    io_context m_ioContext;
    any_io_executor m_ioContextExecutor;
    std::thread m_threadIoContext;
    std::weak_ptr<IClientSession> m_storageSession;
    std::weak_ptr<IClientSession> m_accessSession;
    std::vector<ObjectWithAttributes*> m_objects;
    std::string m_filename = "new_file";
};
} // namespace inklink::draw
