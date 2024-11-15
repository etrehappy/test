#pragma once

#include <SQLiteCpp/SQLiteCpp.h>

#include <boost/asio.hpp>
#include <boost/asio/experimental/awaitable_operators.hpp>
#include <boost/asio/awaitable.hpp> 

#include "logger.h"
#include "settings.h"
#include "event_handler.h"
#include "event_dispatcher.h"
#include "event_queue.h"
#include "database.h"
#include "game_elements.h"

/*!
* @brief There is one DatabaseHandler for all clients in the MVP version so the
*		EventDispatcher uses the kDatabaseClientId to find the handler.
*/
class DatabaseHandler
	: public EventHandler, public std::enable_shared_from_this<EventHandler> 
{
public:
	using b_await_void = boost::asio::awaitable<void>;

	DatabaseHandler(Database&, EventQueue&);
	~DatabaseHandler();
	
	b_await_void ForwardEventToHandler(shrd_event) override;
	void RegisterInDispatcher(EventDispatcher& d) override;

private:

	template<typename T>
	b_await_void HandleClassedEvent(std::shared_ptr<T> event);

	//boost::asio::awaitable<void> CheckLogPas(std::shared_ptr<LogPasClientResponse> event);
	//boost::asio::awaitable<void> GetAvatarList(std::shared_ptr<AvatarListClientRequest> event);

	EventQueue& event_queue_;
	//std::shared_ptr<SQLite::Database> db_read_;
	std::shared_ptr<SQLite::Database> db_read_write_;
};