#pragma once

#include "IObject.h"

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

#include <string>
#include <thread>
#include <vector>

namespace client_network
{
class IClientSession;
} // namespace client_network

namespace inklink::draw
{
class DrawView;

using namespace boost::asio;
} // namespace inklink::draw

namespace inklink::draw
{
class DrawModel
{
public:
    DrawModel();

    ~DrawModel();

    std::string Serialize(int actionType, int figureId, int type);
    void Send(std::string& message);
    void SetFilename(std::string& filename);

    // protected:
    //     void addObject(size_t, size_t, std::vector<Point>&);
    //     void addObject(size_t, Point&, size_t, size_t);
    //     void addObject(size_t, std::string&, Point&, size_t);

private:
    DrawView* m_view = nullptr;
    io_context m_ioContext;
    any_io_executor m_ioContextExecutor;
    std::thread m_threadIoContext;
    std::weak_ptr<client_network::IClientSession> m_storageSession;
    std::weak_ptr<client_network::IClientSession> m_accessSession;
    std::vector<ObjectWithAttributes> m_objects;
    std::string m_filename = "new_file";

    void Deserialize(const std::string& message);
    [[nodiscard]] int GenerateRandomNumber();
};
} // namespace inklink::draw
