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

class IServiceSession;

template <typename T>
concept DoOnAcceptConcept =
        requires(T &&t, boost::system::error_code ec, IServiceSession *session) {
            {
                std::forward<T>(t)(ec, session)
            } -> std::same_as<void>;
        };

template <DoOnAcceptConcept DoOnAccept =
                  std::function<void(boost::system::error_code, IServiceSession *)>>
class BeastWebsocketListener : public IListener,
                               public std::enable_shared_from_this<BeastWebsocketListener>
{
public:
    BeastWebsocketListener() = delete;
    explicit BeastWebsocketListener(
            boost::asio::io_context &, boost::asio::ip::tcp::endpoint,
            std::shared_ptr<ISessionsFactory>,
            DoOnAccept doOnAccept = [](boost::system::error_code, IServiceSession *) {});

    virtual ~BeastWebsocketListener() = default;

    void async_run() override;

private:
    using tcp = boost::asio::ip::tcp;

    void do_accept();

    void fail(boost::system::error_code ec, char const *what);
    void on_accept(boost::system::error_code ec, tcp::socket socket);

    net::io_context &m_ioc;
    tcp::acceptor m_acceptor;
    std::shared_ptr<ISessionsFactory> m_factory;
    DoOnAccept m_doOnAccept;
};

#endif  // _BEASTWEBSOCKETLISTENER_H_
