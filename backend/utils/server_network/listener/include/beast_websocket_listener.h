#pragma once

#include "ilistener.h"

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/linux_error.hpp>
#include <boost/system/system_error.hpp>

#include <concepts>
#include <functional>
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

    ~BeastWebsocketListener() final = default;

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
