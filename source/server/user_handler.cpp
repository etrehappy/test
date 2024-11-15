#include "user_handler.h"
#include "user_handler.ipp" 

UserHandler::UserHandler(EventQueue& q, const std::string& session_id)
    :event_queue_{q}, event_routing_key_{session_id}
{
}

UserHandler::~UserHandler()
{
}

void UserHandler::RegisterInDispatcher(EventDispatcher& d)
{   
	auto register_handler = [this, &d](EventType type) {
		d.RegisterHandler(type, shared_from_this(), event_routing_key_);
		};

	register_handler(EventType::kFirstConnect);
	register_handler(EventType::kSuccessCheckLogPas);
	//register_handler(EventType::kAvatarsDataReady);
	//register_handler(EventType::kFailureCheckLogPas);

    //d.RegisterHandler(EventType::kUserLogPasClientResponse, shared_from_this(), event_routing_key_);
}



boost::asio::awaitable<void> UserHandler::ForwardEventToHandler(shrd_event event)
{
	auto executor = co_await boost::asio::this_coro::executor;

	switch (event->GetType())
	{
	case EventType::kFirstConnect:
        event_queue_.AddEvent(std::make_shared<LoginRequest>(event->GetRoutingKey()));
		break;

	case EventType::kSuccessCheckLogPas:
	{
		boost::asio::co_spawn(
			executor,
			PassClassedEvent<SuccessCheckLogPas>(
				event,
				std::bind(&UserHandler::HandleClassedEvent<SuccessCheckLogPas>, this, std::placeholders::_1),
				EventType::kSuccessCheckLogPas
			),
			boost::asio::detached);
	}
		break;

	case EventType::kFailureCheckLogPas:
	/*Does nothing in the MVP-version. See Networkhendler*/
		break;
	case EventType::kNone:
	case EventType::kEnd:
	default:
		break;
	}

	co_return;
}


//void UserHandler::ConfirmAuthenticate(std::shared_ptr<SuccessCheckLogPas> event)
//{
//
//
//	return;
//}
//
//void UserHandler::RejectAuthenticate(std::shared_ptr<FailureCheckLogPas> event)
//{
//
//	return;
//}



