#pragma once

#include <QGraphicsScene>

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

#include <limits>
#include <random>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

namespace inklink::client_connector
{
class IClientSession;
} // namespace inklink::client_connector

namespace inklink::draw
{

class ObjectWithAttributes;
class GraphicsDrawView;
} // namespace inklink::draw

using namespace boost::asio;

namespace inklink::draw
{
class DrawSceneModel final : public QGraphicsScene
{
    Q_OBJECT

public:
    enum class Mode
    {
        kMove,
        kLine,
        kFreeLine,
        kRectangle,
        kEllipse
    };

public:
    DrawSceneModel(QObject* parent = nullptr);

    ~DrawSceneModel();

    void SetMode(Mode mode);

    void SetEllipseMode();
    void SetFreeLineMode();
    void SetLineMode();
    void SetRectangleMode();

    std::string Serialize(int actionType, int figureId, int type);
    void Send(std::string& message);
    void SetFilename(std::string& filename);

signals:
    void GotNewMsg(const char* msg);
    // DoOnRead(const std::string& msg) { emit GotNewMsg(msg.c_str()); }
    // so that it will be called in "main" thread

    //
    // Q_DECLARE_META_TYPE(DataContainer);
    // qRegisterMetaType(DataContainer); // if you want to use DataContainer in signals/slots
    //

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;

private:
    using IClientSession = client_connector::IClientSession;

private:
    ObjectWithAttributes* CreateNewItem();
    void Deserialize(const std::string& message);
    [[nodiscard]] int GenerateRandomNumber();

    Mode m_currMode;
    std::unordered_map<std::string /*figure_id*/, ObjectWithAttributes*> m_itemsById;

    std::mt19937_64 m_gen;
    std::uniform_int_distribution<int> m_dis{std::numeric_limits<int>::min(), std::numeric_limits<int>::max()};
    GraphicsDrawView* m_view = nullptr;

    io_context m_ioContext;
    any_io_executor m_ioContextExecutor;
    std::thread m_threadIoContext;
    std::weak_ptr<IClientSession> m_storageSession;
    std::weak_ptr<IClientSession> m_accessSession;
    std::vector<ObjectWithAttributes*> m_objects;
    std::string m_filename = "new_file";
};
} // namespace inklink::draw
