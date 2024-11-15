#pragma once

#include <memory>

#include <boost/asio.hpp>
#include <boost/asio/experimental/awaitable_operators.hpp>
#include <boost/asio/awaitable.hpp> 

#include "messages.pb.h"

#include "settings.h"
#include "event_handler.h"
#include "event_dispatcher.h"
#include "event_queue.h"
#include "logger.h"
#include "game_elements.h"

using boost::asio::ip::tcp;
using boost::asio::use_awaitable_t;
using b_tcp_acceptor = use_awaitable_t<>::as_default_on_t<tcp::acceptor>;
using b_tcp_socket = use_awaitable_t<>::as_default_on_t<tcp::socket>;

class ClientSession;

class NetworkHandler : public EventHandler, public std::enable_shared_from_this<EventHandler>
{
public:
	NetworkHandler(std::shared_ptr<b_tcp_socket> s, EventQueue&, const std::string&);
	~NetworkHandler();

	//boost::asio::awaitable<void> Listen();


	boost::asio::awaitable<void> ForwardEventToHandler(shrd_event) override;
	boost::asio::awaitable<void> GetClientResponse(std::shared_ptr<ClientSession> session);
	void RegisterInDispatcher(EventDispatcher& d) override;

private:
	void ClientSessionDisconnect();
	/*!
	* @brief 
	*/
	//template<EventType T>
	boost::asio::awaitable<void> HandleEvent(EventType, ProtoMessage::CommandType);

	template<typename T>
	boost::asio::awaitable<void> HandleClassedEvent(std::shared_ptr<T> event);

	std::shared_ptr<b_tcp_socket> socket_;
	EventQueue& event_queue_;
	const std::string& event_routing_key_;
	std::shared_ptr<const std::string> uuid_{};
	std::shared_ptr<const Avatar> current_avatar_{};


	//uint32_t avatar_id_{};
};


