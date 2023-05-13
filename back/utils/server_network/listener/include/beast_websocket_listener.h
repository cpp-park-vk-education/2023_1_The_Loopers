#ifndef _BEASTWEBSOCKETLISTENER_H_
#define _BEASTWEBSOCKETLISTENER_H_

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/linux_error.hpp>
#include <boost/system/system_error.hpp>
#include <functional>
#include <memory>

#include "ilistener.h"

template <class DoOnAccept>
class BeastWebsocketListener : public IListener,
                               public std::enable_shared_from_this<BeastWebsocketListener>
{
public:
    BeastWebsocketListener() = delete;
    explicit BeastWebsocketListener(boost::asio::io_context &, boost::asio::ip::tcp::endpoint,
                                    std::shared_ptr<ISessionsFactory>, DoOnAccept doOnAccept);
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
