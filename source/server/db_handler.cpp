#include "event_handler.h"
#include "db_handler.h"
#include "db_handler.ipp"

DatabaseHandler::DatabaseHandler(Database& db, EventQueue& q)
	:event_queue_{q}, 
	/*db_read_ {db.GetToRead()}*/ db_read_write_{db.GetToReadAndWrite()}
{
}

DatabaseHandler::~DatabaseHandler()
{
}

void DatabaseHandler::RegisterInDispatcher(EventDispatcher& d)
{
	/* Passed shared_from_this because it used by the EventDispatcher. But in the MVP 
	*   version there is only one DatabaseHandler for all clients. 
	*/
	//d.RegisterHandler(EventType::kUserLogPasClientResponse, shared_from_this(), settings::server::kDatabaseEventRoutingKey);
	//d.RegisterHandler(EventType::kAuthenticateAttempt, shared_from_this(), settings::kDatabaseClientId);

	auto register_handler = [this, &d](EventType type) {
		d.RegisterHandler(type, shared_from_this(), settings::server::kDatabaseEventRoutingKey);
		};

	register_handler(EventType::kUserLogPasClientResponse);
	register_handler(EventType::kUsersAvatarListClientRequest);
	register_handler(EventType::kGetAvatarsData);
	register_handler(EventType::kAddItemInInventory);
	register_handler(EventType::kSaveGame);
	
}

boost::asio::awaitable<void> DatabaseHandler::ForwardEventToHandler(shrd_event event)
{
	auto executor = co_await boost::asio::this_coro::executor;

	switch (event->GetType())
	{
	case EventType::kUserLogPasClientResponse:
	{
		boost::asio::co_spawn(
			executor,
			PassClassedEvent<LogPasClientResponse>(
				event,
				std::bind(&DatabaseHandler::HandleClassedEvent<LogPasClientResponse>, this, std::placeholders::_1),
				EventType::kUserLogPasClientResponse
			),
			boost::asio::detached);
	}
		break;
		
	case EventType::kUsersAvatarListClientRequest:
	{
		boost::asio::co_spawn(
			executor,
			PassClassedEvent<AvatarListClientRequest>(
				event,
				std::bind(&DatabaseHandler::HandleClassedEvent<AvatarListClientRequest>, this, std::placeholders::_1),
				EventType::kUsersAvatarListClientRequest
			),
			boost::asio::detached);
	}
		break;

	case EventType::kGetAvatarsData:
	{
		boost::asio::co_spawn(
			executor,
			PassClassedEvent<GetAvatarsData>(
				event,
				std::bind(&DatabaseHandler::HandleClassedEvent<GetAvatarsData>, this, std::placeholders::_1),
				EventType::kGetAvatarsData
			),
			boost::asio::detached);
	}
		break;
	case EventType::kAddItemInInventory:
	{
		boost::asio::co_spawn(
			executor,
			PassClassedEvent<AddItemInInventory>(
				event,
				std::bind(&DatabaseHandler::HandleClassedEvent<AddItemInInventory>, this, std::placeholders::_1),
				EventType::kAddItemInInventory
			),
			boost::asio::detached);
	}
		break;
	case EventType::kSaveGame:
	{
		boost::asio::co_spawn(
			executor,
			PassClassedEvent<SaveGame>(
				event,
				std::bind(&DatabaseHandler::HandleClassedEvent<SaveGame>, this, std::placeholders::_1),
				EventType::kSaveGame
			),
			boost::asio::detached);
	}
	default:
		break;
	}

	co_return;
}