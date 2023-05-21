#include "beast_websocket_listener.h"

namespace
{
namespace net = boost::asio;
using tcp = net::ip::tcp;
namespace beast = boost::beast;
using error_code = boost::system::error_code;
} // namespace

namespace inklink::server_network
{
// Accepts incoming connections and launches the sessions
template <AcceptCallbackConcept AcceptCallback>
BeastWebsocketListener<AcceptCallback>::BeastWebsocketListener(net::io_context& ioContext,
                                                               const tcp::endpoint& endpoint,
                                                               std::unique_ptr<ISessionsFactory> factory,
                                                               AcceptCallback acceptCallback)
        : IListener{std::move(factory)}, m_ioContext{ioContext}, m_acceptor{ioContext}, m_acceptCallback{acceptCallback}
{
    error_code errorCode;

    // Open the acceptor
    if (m_acceptor.open(endpoint.protocol(), errorCode); errorCode)
    {
        TerminateOnFail(errorCode, "open ");
        return;
    }

    // Allow address reuse
    m_acceptor.set_option(net::socket_base::reuse_address(true), errorCode);
    if (errorCode)
    {
        TerminateOnFail(errorCode, "set_option ");
        return;
    }

    // Bind to the server address
    if (m_acceptor.bind(endpoint, errorCode); errorCode)
    {
        TerminateOnFail(errorCode, "bind ");
        return;
    }

    // Start listening for connections
    m_acceptor.listen(net::socket_base::max_listen_connections, errorCode);
    if (errorCode)
    {
        TerminateOnFail(errorCode, "listen ");
        return;
    }
}

// Start accepting incoming connections
template <AcceptCallbackConcept AcceptCallback>
void BeastWebsocketListener<AcceptCallback>::AsyncRun()
{
    if (m_acceptor.is_open())
    {
        DoAccept();
    }
}

template <AcceptCallbackConcept AcceptCallback>
void BeastWebsocketListener<AcceptCallback>::DoAccept()
{
    m_acceptor.async_accept(
            net::make_strand(m_ioContext),
            beast::bind_front_handler(&BeastWebsocketListener<AcceptCallback>::OnAccept, this->shared_from_this()));
}

template <AcceptCallbackConcept AcceptCallback>
void BeastWebsocketListener<AcceptCallback>::OnAccept(error_code ec, tcp::socket socket)
{
    if (ec)
    {
        m_acceptCallback(ec, nullptr);
    }
    else
    {
        // Create the session and run it
        auto session = m_factory->GetSession(std::move(socket));
        m_acceptCallback(ec, session.get());
        session->RunAsync();
    }

    DoAccept();
}

// Report a Failure
template <AcceptCallbackConcept AcceptCallback>
void BeastWebsocketListener<AcceptCallback>::TerminateOnFail(error_code ec, const std::string&)
{
    // TODO (a.novak) custom error or new error_code with additional info
    throw ec;
}
} // namespace inklink::server_network
