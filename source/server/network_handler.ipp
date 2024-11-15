template<>
boost::asio::awaitable<void> NetworkHandler::HandleClassedEvent(std::shared_ptr<SuccessCheckLogPas> event)
{
	try
	{
		ProtoMessage::GameMessage message{};

		message.set_command(ProtoMessage::CommandType::kAuthSuccess);

		message.mutable_auth_data()->mutable_auth_success()->set_uuid(*event->GetUuid());

		uuid_ = event->GetUuid();

		std::string serialized_message;
		if (!message.SerializeToString(&serialized_message))
		{
			logger::ErrorSerrialisation(event->GetType());
			co_return;
		}

		co_await boost::asio::async_write(*socket_, boost::asio::buffer(serialized_message));
	}
	catch (const std::exception& e)
	{
		MVP_LOG_EXEPTION(e);
	}

	co_return;
}


template<>
boost::asio::awaitable<void> NetworkHandler::HandleClassedEvent(std::shared_ptr<AvatarListIsReady> event)
{
	try
	{
		ProtoMessage::GameMessage message{};

		message.set_command(ProtoMessage::CommandType::kUsersAvatarListServerResponse);		

		for (const auto& avatar : *event->GetAvatars())
		{
			auto message_avatar = message.mutable_avatar_list_data()->add_avatar_list();
			message_avatar->set_id(avatar.first);
			message_avatar->set_name(avatar.second);
		}

		std::string serialized_message;
		if (!message.SerializeToString(&serialized_message))
		{
			logger::ErrorSerrialisation(event->GetType());
			co_return;
		}

		co_await boost::asio::async_write(*socket_, boost::asio::buffer(serialized_message));
	}
	catch (const std::exception& e)
	{
		MVP_LOG_EXEPTION(e);
	}

	co_return;
}


template<>
boost::asio::awaitable<void> NetworkHandler::HandleClassedEvent(std::shared_ptr<StartGameServerResponseSucsses> event)
{
	try
	{
		ProtoMessage::GameMessage message{};

		message.set_command(ProtoMessage::CommandType::kStartGameServerResponse);
		message.mutable_start_game()->set_is_server_ready(true);

		auto message_avatar = message.mutable_start_game()->mutable_avatar();
		current_avatar_ = event->GetConstAvatar();
		std::cout << "\n current_avatar_: " << current_avatar_;
		if (!current_avatar_)
		{
			throw std::invalid_argument{logger::kErrExeptEmptyVariable};
			/*TODO: error event for client*/
			co_return;
		}

		message_avatar->set_id(current_avatar_->GetId());
		message_avatar->set_name(current_avatar_->GetName());
		message_avatar->set_race(current_avatar_->GetRaceId());
		message_avatar->set_class_(current_avatar_->GetClassId());
		message_avatar->set_level(current_avatar_->GetLevel());
		message_avatar->set_experience(current_avatar_->GetExperience());
		message_avatar->set_health(current_avatar_->GetHealth());
		message_avatar->set_mana(current_avatar_->GetMana());
		message_avatar->set_strength(current_avatar_->GetStrength());
		message_avatar->set_dexterity(current_avatar_->GetDexterity());
		message_avatar->set_intelligence(current_avatar_->GetIntelligence());
		message_avatar->set_location_id(current_avatar_->GetLocationId());
		message_avatar->set_pos_x(current_avatar_->GetPosition().first);
		message_avatar->set_pos_y(current_avatar_->GetPosition().second);

		std::string serialized_message;
		if (!message.SerializeToString(&serialized_message))
		{
			logger::ErrorSerrialisation(event->GetType());
			/*TODO: error event for client*/
			co_return;
		}

		co_await boost::asio::async_write(*socket_, boost::asio::buffer(serialized_message));
	}
	catch (const std::exception& e)
	{
		MVP_LOG_EXEPTION(e);
	}

	co_return;
}

template<>
boost::asio::awaitable<void> NetworkHandler::HandleClassedEvent(std::shared_ptr<BroadcastMessage> event)
{
	try
	{
		ProtoMessage::GameMessage message{};

		message.set_command(ProtoMessage::CommandType::kGetCommonMessage);

		const std::string& text = event->GetEventMessage()->chat_data().text_message();
		const std::string& sender = current_avatar_->GetName();
		message.mutable_chat_data()->set_text_message(text);
		message.mutable_chat_data()->set_sender_avatar_name(sender);


		std::string serialized_message;
		if (!message.SerializeToString(&serialized_message))
		{
			logger::ErrorSerrialisation(event->GetType());
			co_return;
		}

		co_await boost::asio::async_write(*socket_, boost::asio::buffer(serialized_message));
	}
	catch (const std::exception& e)
	{
		MVP_LOG_EXEPTION(e);
	}

	co_return;
}

template<>
boost::asio::awaitable<void> NetworkHandler::HandleClassedEvent(std::shared_ptr<ExitGameResponse> event)
{
	try
	{
		ProtoMessage::GameMessage message{};

		message.set_command(ProtoMessage::CommandType::kExitGameServerResponse);
		message.mutable_exit_game()->set_is_server_ready(true);

		std::string serialized_message;
		if (!message.SerializeToString(&serialized_message))
		{
			logger::ErrorSerrialisation(event->GetType());
			co_return;
		}

		event_queue_.AddEvent(std::make_shared<DisconnectSession>(event_routing_key_));
		co_await boost::asio::async_write(*socket_, boost::asio::buffer(serialized_message));
		
		/*ClientSessionDisconnect();* !!! */


	}
	catch (const std::exception& e)
	{
		MVP_LOG_EXEPTION(e);
	}

	co_return;
}






//template<>
//boost::asio::awaitable<void> NetworkHandler::HandleEvent<EventType::kUserLogPasServerRequest>()
//{
//	ProtoMessage::GameMessage message{};
//
//	message.set_command(ProtoMessage::CommandType::kLoginPasRequest);
//
//	std::string serialized_message;
//	if (!message.SerializeToString(&serialized_message))
//	{
//		logger::ErrorSerrialisation(EventType::kUserLogPasServerRequest);
//		co_return;
//	}
//
//	co_await boost::asio::async_write(*socket_, boost::asio::buffer(serialized_message));
//}

//template<>
//boost::asio::awaitable<void> NetworkHandler::HandleEvent<EventType::kSuccessCheckLogPas>()
//{
//
//	ProtoMessage::GameMessage message{};
//
//	message.set_command(ProtoMessage::CommandType::kAuthSuccess);
//
//	std::string serialized_message;
//	if (!message.SerializeToString(&serialized_message))
//	{
//		logger::ErrorSerrialisation(EventType::kSuccessCheckLogPas);
//		co_return;
//	}
//
//	co_await boost::asio::async_write(*socket_, boost::asio::buffer(serialized_message));
//}
//
//template<>
//boost::asio::awaitable<void> NetworkHandler::HandleEvent<EventType::kFailureCheckLogPas>()
//{
//	//auto self = shared_from_this(); // !!!
//
//	//ProtoMessage::GameMessage message{};
//
//	//message.set_command(ProtoMessage::CommandType::kAuthFailure);
//
//	//std::string serialized_message;
//	//if (!message.SerializeToString(&serialized_message))
//	//{
//	//	logger::ErrorSerrialisation(EventType::kUserLogPasServerRequest);
//	//	co_return;
//	//}
//
//	co_await boost::asio::async_write(*socket_, boost::asio::buffer(serialized_message));
//}