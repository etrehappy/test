#pragma once

#include "settings.h"
#include "event.h"
#include "event_handler.h"
#include "event_dispatcher.h"
#include "event_queue.h"
#include "logger.h"

#include "user_handler.h"
#include "network_handler.h"
#include "game_handler.h"

#include <boost/asio.hpp>
#include <boost/asio/experimental/awaitable_operators.hpp>
#include <boost/asio/awaitable.hpp> 

#include <memory> 

using boost::asio::ip::tcp;
using boost::asio::use_awaitable_t;
using b_tcp_acceptor = use_awaitable_t<>::as_default_on_t<tcp::acceptor>;
using b_tcp_socket = use_awaitable_t<>::as_default_on_t<tcp::socket>;

class ClientSession : public std::enable_shared_from_this<ClientSession> 
{
public:
    ClientSession(std::shared_ptr<b_tcp_socket> socket, EventQueue& q, EventDispatcher& d);

    boost::asio::awaitable<void> Start();
    bool IsActive();
    /*
    NetworkHandler::HandleClassedEvent(std::shared_ptr<ExitGameResponse> event)
    */
    void SetInactive();

private:
   // boost::asio::awaitable<void> HandleConnection();

    std::shared_ptr<b_tcp_socket> socket_;
    EventQueue& event_queue_;
    EventDispatcher& dispatcher_;
    const std::string client_ip_; // !!! где нужен?
    const std::string session_id_;
    bool active_;

    std::shared_ptr<UserHandler> user_handler_;
    std::shared_ptr<NetworkHandler> network_handler_;
    std::shared_ptr<GameHandler> game_handler_;

    /// не моё

   // std::string CheckOrCreateUUID();


};
