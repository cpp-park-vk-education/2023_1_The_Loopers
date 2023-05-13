#ifndef _BEASTWEBSOCKETLISTENER_H_
#define _BEASTWEBSOCKETLISTENER_H_

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/linux_error.hpp>
#include <boost/system/system_error.hpp>
#include <concepts>
#include <functional>
#include <memory>

#include "ilistener.h"

namespace inklink_service_session
{
class IServiceSession;
}  // namespace inklink_service_session

namespace inklink_listener
{
template <typename T>
concept DoOnAcceptConcept = requires(T&& t, boost::system::error_code ec,
                                     inklink_service_session::IServiceSession* session) {
    {
        std::forward<T>(t)(ec, session)
    } -> std::same_as<void>;
};

template <DoOnAcceptConcept DoOnAccept = std::function<void(
                  boost::system::error_code, inklink_service_session::IServiceSession*)>>
class BeastWebsocketListener : public IListener,
                               public std::enable_shared_from_this<BeastWebsocketListener>
{
public:
    using IServiceSession = inklink_service_session::IServiceSession;

    BeastWebsocketListener() = delete;
    explicit BeastWebsocketListener(
            boost::asio::io_context&, boost::asio::ip::tcp::endpoint,
            std::shared_ptr<ISessionsFactory>,
            DoOnAccept doOnAccept = [](boost::system::error_code, IServiceSession*) {});

    virtual ~BeastWebsocketListener() = default;

    void async_run() override;

private:
    using tcp = boost::asio::ip::tcp;

    void do_accept();

    void fail(boost::system::error_code ec, char const* what);
    void on_accept(boost::system::error_code ec, tcp::socket socket);

    net::io_context& m_ioc;
    tcp::acceptor m_acceptor;
    DoOnAccept m_doOnAccept;
};
}  // namespace inklink_listener

#endif  // _BEASTWEBSOCKETLISTENER_H_
