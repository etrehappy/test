#include "session.h"
#include "network_handler.h"
#include "network_handler.ipp"


////////////////////////////////////////////////////////////////////////////////

NetworkHandler::NetworkHandler(std::shared_ptr<b_tcp_socket> s, EventQueue& q, const std::string& session_id)
    :socket_{s}, event_queue_{q}, event_routing_key_{session_id}
{
}

NetworkHandler::~NetworkHandler()
{
	if (socket_ && socket_->is_open()) 
	{
		socket_->close();
	}
}

void NetworkHandler::RegisterInDispatcher(EventDispatcher& d)
{
	auto register_handler = [this, &d](EventType type) {
		d.RegisterHandler(type, shared_from_this(), event_routing_key_);
		};

	register_handler(EventType::kUserLogPasServerRequest);
	register_handler(EventType::kSuccessCheckLogPas);
	register_handler(EventType::kFailureCheckLogPas);
	register_handler(EventType::kAvatarListIsReady); 
	register_handler(EventType::kStartGameServerResponseSucsses);	
	register_handler(EventType::kBroadcastMessage);
	register_handler(EventType::kExitGameResponse);

}

void NetworkHandler::ClientSessionDisconnect()
{
	/*Stop Game*/
	socket_->close();
}

boost::asio::awaitable<void> NetworkHandler::GetClientResponse(std::shared_ptr<ClientSession> session)
{	
	auto executor = co_await boost::asio::this_coro::executor;
	auto self = shared_from_this(); // !!! нужно ли

	while (session->IsActive())
	{
	try
	{
		char data[settings::kBufferSize];

		size_t length = co_await socket_->async_read_some(boost::asio::buffer(data));

		std::string received_data(data, length);

		std::shared_ptr<ProtoMessage::GameMessage> message = std::make_shared<ProtoMessage::GameMessage>();
		message->ParseFromString(received_data);

		using pt = ProtoMessage::CommandType;

		switch (message->command())
		{
		case pt::kLoginPasResponse:
			event_queue_.AddEvent(std::make_shared<LogPasClientResponse>(message, event_routing_key_));
			break;

		case pt::kUsersAvatarListClientRequest:
			event_queue_.AddEvent(std::make_shared<AvatarListClientRequest>(event_routing_key_, *uuid_));
			break;

		case pt::kStartGameClientRequest:
			
			event_queue_.AddEvent(std::make_shared<GetAvatarsData>(event_routing_key_, 
				message->start_game().id_selected_avatar()));
			event_queue_.AddEvent(std::make_shared<StartGameClientRequest>(event_routing_key_));
			break;

		case pt::kAddItemInInventory:
			event_queue_.AddEvent(std::make_shared<AddItemInInventory>(
				static_cast<ItemType>(message->item()), current_avatar_->GetId())
			);
			break;
		case pt::kSendToCommonChat:
			event_queue_.AddEvent(std::make_shared<BroadcastMessage>(message, event_routing_key_));
			break;
		case pt::kExitGameClientRequest:
			event_queue_.AddEvent(std::make_shared<ExitGameRequest>(message, event_routing_key_));
			break;		
		default:
			break;
		}
	}/*try*/
	catch (const boost::system::system_error& e)
	{
		if (e.code() == boost::asio::error::eof || e.code() == boost::asio::error::connection_reset) 
		{
			auto client_ip{socket_->remote_endpoint().address().to_string()
				+ ":" + std::to_string(socket_->remote_endpoint().port())}; /* !!! Можно из session взять? */
			
			logger::ClientDisconnect(client_ip);
			
			session->SetInactive();
			continue;
		}
		else
		{
			MVP_LOG_EXEPTION(e);
		}
	}
	catch (const std::exception& e)
	{
		MVP_LOG_EXEPTION(e)
	}
		
	}/*while*/
}



boost::asio::awaitable<void> NetworkHandler::ForwardEventToHandler(shrd_event event)
{
	auto executor = co_await boost::asio::this_coro::executor;

	auto self = shared_from_this(); // !!!

	switch (event->GetType())
	{
	case EventType::kUserLogPasServerRequest:
		boost::asio::co_spawn(executor, 
			HandleEvent(EventType::kUserLogPasServerRequest, ProtoMessage::CommandType::kLoginPasRequest),
			boost::asio::detached);
		break;

	case EventType::kSuccessCheckLogPas:
	{
		boost::asio::co_spawn(
			executor,
			PassClassedEvent<SuccessCheckLogPas>(
				event,
				std::bind(&NetworkHandler::HandleClassedEvent<SuccessCheckLogPas>, this, std::placeholders::_1),
				EventType::kSuccessCheckLogPas
			),
			boost::asio::detached);
	}
		break;
	case EventType::kFailureCheckLogPas:
		boost::asio::co_spawn(executor, 
			HandleEvent(EventType::kSuccessCheckLogPas, ProtoMessage::CommandType::kAuthFailure),
			boost::asio::detached);
		break;

	case EventType::kAvatarListIsReady:
	{
		boost::asio::co_spawn(
			executor,
			PassClassedEvent<AvatarListIsReady>(
				event,
				std::bind(&NetworkHandler::HandleClassedEvent<AvatarListIsReady>, this, std::placeholders::_1),
				EventType::kAvatarListIsReady
			),
			boost::asio::detached);
	}
		break;
	case EventType::kStartGameServerResponseSucsses:
	{
		boost::asio::co_spawn(
			executor,
			PassClassedEvent<StartGameServerResponseSucsses>(
				event,
				std::bind(&NetworkHandler::HandleClassedEvent<StartGameServerResponseSucsses>, this, std::placeholders::_1),
				EventType::kStartGameServerResponseSucsses
			),
			boost::asio::detached);
	}
		break;
	case EventType::kBroadcastMessage:
	{
		boost::asio::co_spawn(
			executor,
			PassClassedEvent<BroadcastMessage>(
				event,
				std::bind(&NetworkHandler::HandleClassedEvent<BroadcastMessage>, this, std::placeholders::_1),
				EventType::kBroadcastMessage
			),
			boost::asio::detached);
	}
	case EventType::kExitGameResponse:
	{
		boost::asio::co_spawn(
			executor,
			PassClassedEvent<ExitGameResponse>(
				event,
				std::bind(&NetworkHandler::HandleClassedEvent<ExitGameResponse>, this, std::placeholders::_1),
				EventType::kExitGameResponse
			),
			boost::asio::detached);
	}
	break;
	default:
		break;
	}
}


boost::asio::awaitable<void> NetworkHandler::HandleEvent(EventType type, ProtoMessage::CommandType command)
{
	ProtoMessage::GameMessage message{};

	message.set_command(command);

	std::string serialized_message;
	if (!message.SerializeToString(&serialized_message))
	{
		logger::ErrorSerrialisation(type);
		co_return;
	}

	co_await boost::asio::async_write(*socket_, boost::asio::buffer(serialized_message));
}
//
//boost::asio::awaitable<void> DatabaseHandler::GetAvatarList(std::shared_ptr<AvatarListClientRequest> event)
//{

//}