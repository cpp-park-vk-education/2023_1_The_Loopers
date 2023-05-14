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
concept DoOnAcceptConcept = requires(T&& t, boost::system::error_code ec, IServiceSession* session) {
    {
        std::forward<T>(t)(ec, session)
    } -> std::same_as<void>;
};

template <DoOnAcceptConcept DoOnAccept = std::function<void(boost::system::error_code, IServiceSession*)>>
class BeastWebsocketListener : public IListener, public std::enable_shared_from_this<BeastWebsocketListener<DoOnAccept>>
{
public:
    BeastWebsocketListener() = delete;
    explicit BeastWebsocketListener(
            boost::asio::io_context&, boost::asio::ip::tcp::endpoint, std::shared_ptr<ISessionsFactory>,
            DoOnAccept doOnAccept = [](boost::system::error_code, IServiceSession*) {});

    ~BeastWebsocketListener() final = default;

    void AsyncRun() final;

private:
    using tcp = boost::asio::ip::tcp;

    void DoAccept();

    void Fail(boost::system::error_code ec, char const* what);
    void OnAccept(boost::system::error_code ec, tcp::socket socket);

    boost::asio::io_context& m_ioc;
    tcp::acceptor m_acceptor;
    DoOnAccept m_doOnAccept;
};
} // namespace inklink::server_network
