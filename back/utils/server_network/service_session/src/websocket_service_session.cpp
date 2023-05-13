#include "websocket_service_session.h"

#include <internal_sessions_manager.h>

#include <algorithm>

namespace inklink_service_session
{
template <Fun_StringErrorCodeSession DoOnRead, Do_ErrorCodeAndSession_Concept DoOnAccept,
          Do_ErrorCode_Concept DoOnWrite>
WebsocketServiceSession<DoOnRead, DoOnAccept, DoOnWrite>::WebsocketServiceSession(
        net::ip::tcp::socket &&socket, DoOnRead doOnRead, DoOnAccept doOnAccept,
        DoOnWrite doOnWrite)
        : IServiceSession(), m_doOnRead{doOnRead}, m_doOnAccept{doOnAccept}, m_doOnWrite{doOnWrite}
{
    m_endpoint.address = socket.remote_endpoint().address().to_string();
    m_endpoint.port = socket.remote_endpoint().port();
    m_ws = websocket::stream<tcp::socket>(std::move(socket)),
}

template <Fun_StringErrorCodeSession DoOnRead, Do_ErrorCodeAndSession_Concept DoOnAccept,
          Do_ErrorCode_Concept DoOnWrite>
WebsocketServiceSession<DoOnRead, DoOnAccept, DoOnWrite>::~WebsocketServiceSession()
{
    m_manager->RemoveSession(this);
}

template <Fun_StringErrorCodeSession DoOnRead, Do_ErrorCodeAndSession_Concept DoOnAccept,
          Do_ErrorCode_Concept DoOnWrite>
void WebsocketServiceSession<DoOnRead, DoOnAccept, DoOnWrite>::RunAsync()
{
    // We need to be executing within a strand to perform async operations
    // on the I/O objects in this session. Although not strictly necessary
    // for single-threaded contexts, this example code is written to be
    // thread-safe by default.
    net::dispatch(m_ws.get_executor(),
                  beast::bind_front_handler(&WebsocketServiceSession::on_run, shared_from_this()));
}

template <Fun_StringErrorCodeSession DoOnRead, Do_ErrorCodeAndSession_Concept DoOnAccept,
          Do_ErrorCode_Concept DoOnWrite>
void WebsocketServiceSession<DoOnRead, DoOnAccept, DoOnWrite>::OnRun()
{
    // Set suggested timeout settings for the websocket
    m_ws.set_option(websocket::stream_base::timeout::suggested(beast::role_type::server));

    // Set a decorator to change the Server of the handshake
    m_ws.set_option(websocket::stream_base::decorator(
            [](websocket::response_type &res)
            {
                res.set(http::field::server, std::string(BOOST_BEAST_VERSION_STRING) +
                                                     " inklink-simultaneous-access-service");
            }));
    // Accept the websocket handshake
    m_ws.async_accept(
            beast::bind_front_handler(&WebsocketServiceSession::OnAccept, shared_from_this()));
}

template <Fun_StringErrorCodeSession DoOnRead, Do_ErrorCodeAndSession_Concept DoOnAccept,
          Do_ErrorCode_Concept DoOnWrite>
void WebsocketServiceSession<DoOnRead, DoOnAccept, DoOnWrite>::Send(const std::string &message)
{
    auto ss = std::make_shared<std::string const>(message);
    // Always add to queue
    m_queue.push_back(ss);

    // Are we already writing?
    if (m_queue.size() > 1)
        return;

    // We are not currently writing, so send this immediately
    m_bufferSending = net::buffer(*m_queue.front());
    m_ws.async_write(m_bufferSending, beast::bind_front_handler(&WebsocketServiceSession::OnWrite,
                                                                shared_from_this()));
}

template <Fun_StringErrorCodeSession DoOnRead, Do_ErrorCodeAndSession_Concept DoOnAccept,
          Do_ErrorCode_Concept DoOnWrite>
void WebsocketServiceSession<DoOnRead, DoOnAccept, DoOnWrite>::OnAccept(
        boost::system::error_code ec)
{
    m_doOnAccept(ec, this);

    if (ec)
    {
        // destructor is called automatically, because no more shared_ptr points to this
        return;
    }

    // Read a message
    DoRead();
}

template <Fun_StringErrorCodeSession DoOnRead, Do_ErrorCodeAndSession_Concept DoOnAccept,
          Do_ErrorCode_Concept DoOnWrite>
void WebsocketServiceSession<DoOnRead, DoOnAccept, DoOnWrite>::DoRead()
{
    // Read a message into our buffer
    ws_.async_read(buffer_,
                   beast::bind_front_handler(&WebsocketServiceSession::OnRead, shared_from_this()));
}

template <Fun_StringErrorCodeSession DoOnRead, Do_ErrorCodeAndSession_Concept DoOnAccept,
          Do_ErrorCode_Concept DoOnWrite>
void WebsocketServiceSession<DoOnRead, DoOnAccept, DoOnWrite>::OnRead(boost::system::error_code ec,
                                                                      std::size_t)
{
    m_doOnRead(beast::buffers_to_string(m_buffer.data()), ec, this);

    // TODO (a.novak) some errors should be ok and session should not be destructed, but in my
    // practice connection should be closed not only after websocket::error::closed (at least in
    // version 1.72)
    if (ec)
    {
        // this will be deleted automatically, because no more shared_ptr will point to this
        return;
    }

    // clean buffer: if there was smth extra, it was garbage (it supposed to
    // contain nothing before read)
    m_buffer.consume(m_buffer.size());

    DoRead();
}

template <Fun_StringErrorCodeSession DoOnRead, Do_ErrorCodeAndSession_Concept DoOnAccept,
          Do_ErrorCode_Concept DoOnWrite>
void WebsocketServiceSession<DoOnRead, DoOnAccept, DoOnWrite>::OnWrite(boost::system::error_code ec,
                                                                       std::size_t)
{
    m_DoOnWrite(ec);
    if (ec)
    {
        return;
    }

    // Remove the string from the queue
    m_queue.pop_front();

    // Send the next message if any
    if (!m_queue.empty())
    {
        m_bufferSending = net::buffer(*m_queue.front());
        m_ws.async_write(
                m_bufferSending,
                beast::bind_front_handler(&WebsocketServiceSession::OnWrite, shared_from_this()));
    }
}

}  // namespace inklink_service_session
