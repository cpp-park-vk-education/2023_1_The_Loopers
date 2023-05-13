#include "beast_websocket_listener.h"

namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;
namespace beast = boost::beast;

// Accepts incoming connections and launches the sessions
BeastWebsocketListener::BeastWebsocketListener(net::io_context &ioc, tcp::endpoint endpoint,
                                               std::shared_ptr<ISessionsFactory> factory,
                                               DoOnAccept doOnAccept)
        : m_ioc{ioc}, m_acceptor{ioc}, m_factory{factory}, m_doOnAccept{doOnAccept}
{
    boost::system::error_code ec;

    // Open the acceptor
    m_acceptor.open(endpoint.protocol(), ec);
    if (ec)
    {
        fail(ec, "open");
        return;
    }

    // Allow address reuse
    m_acceptor.set_option(net::socket_base::reuse_address(true));
    if (ec)
    {
        fail(ec, "set_option");
        return;
    }

    // Bind to the server address
    m_acceptor.bind(endpoint, ec);
    if (ec)
    {
        fail(ec, "bind");
        return;
    }

    // Start listening for connections
    m_acceptor.listen(net::socket_base::max_listen_connections, ec);
    if (ec)
    {
        fail(ec, "listen");
        return;
    }
}

// Start accepting incoming connections
void BeastWebsocketListener::async_run()
{
    if (!m_acceptor.is_open())
        return;
    do_accept();
}

void BeastWebsocketListener::do_accept()
{
    m_acceptor.async_accept(
            net::make_strand(m_ioc),
            beast::bind_front_handler(&BeastWebsocketListener::on_accept, shared_from_this()));
}

void BeastWebsocketListener::on_accept(boost::system::error_code ec, tcp::socket socket)
{
    if (ec)
    {
        doOnAccept(ec, nullptr);
    }
    else
    {
        // Create the session and run it
        auto session = m_factory->GetSession(std::move(socket));
        doOnAccept(ec, session.get());
        session->run_async();
    }

    do_accept();
}

// Report a failure
void BeastWebsocketListener::fail(boost::system::error_code ec, char const *what)
{
    // Don't report on canceled operations
    if (ec == net::error::operation_aborted)
        return;
    // fail(ec, what, plugin_);
}
