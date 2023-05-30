#pragma once

#include "ilistener.h"

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/linux_error.hpp>
#include <boost/system/system_error.hpp>

#include <concepts>
#include <functional>
#include <iostream>
#include <memory>

namespace inklink::server_network
{
class IServiceSession;

template <typename T>
concept AcceptCallbackConcept = requires(T&& t, boost::system::error_code ec, IServiceSession* session) {
    {
        std::forward<T>(t)(ec, session)
    } -> std::same_as<void>;
};

template <AcceptCallbackConcept AcceptCallback = std::function<void(boost::system::error_code, IServiceSession*)>>
class BeastWebsocketListener final : public IListener,
                                     public std::enable_shared_from_this<BeastWebsocketListener<AcceptCallback>>
{
    using tcp = boost::asio::ip::tcp;
    using error_code = boost::system::error_code;

public:
    BeastWebsocketListener() = delete;
    // clang-format off
    BeastWebsocketListener(
            boost::asio::io_context&, 
            const tcp::endpoint&, 
            std::unique_ptr<ISessionsFactory>,
            AcceptCallback acceptCallback = [](error_code, IServiceSession*) {});
    // clang-format on

    ~BeastWebsocketListener() final
    {
        std::cout << "Listener destructor " << __LINE__ << std::endl;
    }

    void AsyncRun() final;

private:
    void DoAccept();

    void TerminateOnFail(error_code, const std::string& prefix);
    void OnAccept(error_code, tcp::socket socket);

    boost::asio::io_context& m_ioContext;
    tcp::acceptor m_acceptor;
    AcceptCallback m_acceptCallback;
};
} // namespace inklink::server_network

namespace inklink::server_network
{
// Accepts incoming connections and launches the sessions
template <AcceptCallbackConcept AcceptCallback>
inline BeastWebsocketListener<AcceptCallback>::BeastWebsocketListener(boost::asio::io_context& ioContext,
                                                                      const tcp::endpoint& endpoint,
                                                                      std::unique_ptr<ISessionsFactory> factory,
                                                                      AcceptCallback acceptCallback)
        : IListener{std::move(factory)}, m_ioContext{ioContext}, m_acceptor{ioContext}, m_acceptCallback{acceptCallback}
{
    error_code errorCode{};

    // Open the acceptor
    if (m_acceptor.open(endpoint.protocol(), errorCode); errorCode)
    {
        TerminateOnFail(errorCode, "open ");
        std::cout << "Error ocurred while opening acceptor " << errorCode.message() << __LINE__ << std::endl;
        return;
    }

    // Allow address reuse
    m_acceptor.set_option(boost::asio::socket_base::reuse_address(true), errorCode);
    if (errorCode)
    {
        TerminateOnFail(errorCode, "set_option ");
        std::cout << "Error occured while allowing address reuse " << errorCode.message() << __LINE__ << std::endl;
        return;
    }

    // Bind to the server address
    if (m_acceptor.bind(endpoint, errorCode); errorCode)
    {
        TerminateOnFail(errorCode, "bind ");
        std::cout << "Error occured while Binding to the server address " << errorCode.message() << __LINE__
                  << std::endl;
        return;
    }

    // Start listening for connections
    m_acceptor.listen(boost::asio::socket_base::max_listen_connections, errorCode);
    if (errorCode)
    {
        TerminateOnFail(errorCode, "listen ");
        std::cout << "Error occured while starting listening " << errorCode.message() << __LINE__ << std::endl;
        return;
    }
}

// Start accepting incoming connections
template <AcceptCallbackConcept AcceptCallback>
inline void BeastWebsocketListener<AcceptCallback>::AsyncRun()
{
    std::cout << "Starting accepting " << __LINE__ << std::endl;
    if (m_acceptor.is_open())
    {
        DoAccept();
    }
}

template <AcceptCallbackConcept AcceptCallback>
inline void BeastWebsocketListener<AcceptCallback>::DoAccept()
{
    std::cout << "DoAccept " << __LINE__ << std::endl;
    m_acceptor.async_accept(boost::asio::make_strand(m_ioContext),
                            boost::beast::bind_front_handler(&BeastWebsocketListener<AcceptCallback>::OnAccept,
                                                             this->shared_from_this()));
}

template <AcceptCallbackConcept AcceptCallback>
inline void BeastWebsocketListener<AcceptCallback>::OnAccept(error_code ec, tcp::socket socket)
{
    if (ec)
    {
        std::cout << "accepted with error " << ec.message() << __LINE__ << std::endl;
        m_acceptCallback(ec, nullptr);
    }
    else
    {
        std::cout << "accepted successfully " << ec.message() << __LINE__ << std::endl;
        // Create the session and run it
        auto session = m_factory->GetSession(std::move(socket));
        m_acceptCallback(ec, session.get());
        session->RunAsync();
    }

    DoAccept();
}

// Report a Failure
template <AcceptCallbackConcept AcceptCallback>
inline void BeastWebsocketListener<AcceptCallback>::TerminateOnFail(error_code ec, const std::string&)
{
    // TODO (a.novak) custom error or new error_code with additional info
    throw ec;
}
} // namespace inklink::server_network
