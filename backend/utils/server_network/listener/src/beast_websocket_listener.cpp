#include "beast_websocket_listener.h"

namespace inklink::server_network
{
namespace net = boost::asio;
using tcp = net::ip::tcp;
namespace beast = boost::beast;

// Accepts incoming connections and launches the sessions
template <DoOnAcceptConcept DoOnAccept>
BeastWebsocketListener<DoOnAccept>::BeastWebsocketListener(net::io_context &ioc, const tcp::endpoint &endpoint,
                                                           std::shared_ptr<ISessionsFactory> factory,
                                                           DoOnAccept doOnAccept)
        : IListener(std::move(factory)), m_ioc{ioc}, m_acceptor{ioc}, m_doOnAccept{doOnAccept}
{
    error_code ec;

    // Open the acceptor
    m_acceptor.open(endpoint.protocol(), ec);
    if (ec)
    {
        Fail(ec, "open");
        return;
    }

    // Allow address reuse
    m_acceptor.set_option(net::socket_base::reuse_address(true));
    if (ec)
    {
        Fail(ec, "set_option");
        return;
    }

    // Bind to the server address
    m_acceptor.bind(endpoint, ec);
    if (ec)
    {
        Fail(ec, "bind");
        return;
    }

    // Start listening for connections
    m_acceptor.listen(net::socket_base::max_listen_connections, ec);
    if (ec)
    {
        Fail(ec, "listen");
        return;
    }
}

// Start accepting incoming connections
template <DoOnAcceptConcept DoOnAccept>
void BeastWebsocketListener<DoOnAccept>::AsyncRun()
{
    if (!m_acceptor.is_open())
    {
        return;
    }
    DoAccept();
}

template <DoOnAcceptConcept DoOnAccept>
void BeastWebsocketListener<DoOnAccept>::DoAccept()
{
    m_acceptor.async_accept(
            net::make_strand(m_ioc),
            beast::bind_front_handler(&BeastWebsocketListener<DoOnAccept>::on_accept, this->shared_from_this()));
}

template <DoOnAcceptConcept DoOnAccept>
void BeastWebsocketListener<DoOnAccept>::OnAccept(boost::system::error_code ec, tcp::socket socket)
{
    if (ec)
    {
        m_doOnAccept(ec, nullptr);
    }
    else
    {
        // Create the session and run it
        auto session = m_factory->GetSession(std::move(socket));
        doOnAccept(ec, session.get());
        session->RunAsync();
    }

    DoAccept();
}

// Report a Failure
template <DoOnAcceptConcept DoOnAccept>
void BeastWebsocketListener<DoOnAccept>::Fail(boost::system::error_code ec, char const *)
{
    // Don't report on canceled operations
    if (ec == net::error::operation_aborted)
    {
        return;
    }
    // Fail(ec, what, plugin_);
}
} // namespace inklink::server_network
