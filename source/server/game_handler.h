#pragma once

#include "logger.h"
#include "settings.h"
#include "event_handler.h"
#include "event_dispatcher.h"
#include "event_queue.h"
#include <vector>

#include <boost/asio.hpp>
#include <boost/asio/experimental/awaitable_operators.hpp>
#include <boost/asio/awaitable.hpp> 




class GameHandler : public EventHandler, public std::enable_shared_from_this<GameHandler>
{
public:
	GameHandler(EventQueue&, const std::string&);
	~GameHandler();

	boost::asio::awaitable<void> ForwardEventToHandler(shrd_event) override;
	void RegisterInDispatcher(EventDispatcher& d) override;


private:

	EventQueue& event_queue_;
	const std::string& event_routing_key_;
	std::shared_ptr<Avatar> current_avatar_{nullptr};
	std::shared_ptr<Location> current_location_{nullptr}; // !!!
	std::vector<Npc> npc_container_{}; // !!!

	template<typename T>
	boost::asio::awaitable<void> HandleClassedEvent(std::shared_ptr<T> event);

};
