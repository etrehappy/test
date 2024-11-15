#pragma once

#include <memory>

#include "logger.h"
#include "settings.h"
#include "event_handler.h"
#include "event_dispatcher.h"
#include "network_handler.h" // !!!
#include "game_elements.h"

#include <SQLiteCpp/SQLiteCpp.h>

#include <boost/asio.hpp>
#include <boost/asio/experimental/awaitable_operators.hpp>
#include <boost/asio/awaitable.hpp> 




class UserHandler : public EventHandler, public std::enable_shared_from_this<UserHandler>
{
public:
	UserHandler(EventQueue&, const std::string&);
	~UserHandler();

	boost::asio::awaitable<void> ForwardEventToHandler(shrd_event) override;
	void RegisterInDispatcher(EventDispatcher& d) override;
	


private:

	//EventDispatcher& dispatcher_;
	EventQueue& event_queue_;
	const std::string& event_routing_key_;
	
	template<typename T>
	boost::asio::awaitable<void> HandleClassedEvent(std::shared_ptr<T> event);

	//void ConfirmAuthenticate(std::shared_ptr<SuccessCheckLogPas> event);
	//void RejectAuthenticate(std::shared_ptr<FailureCheckLogPas> event);

	
	bool logged_in_{false}; //!!!
	std::shared_ptr<const std::string> uuid_{};
};


	