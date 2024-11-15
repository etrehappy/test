#include "logger.h"

template<>
boost::asio::awaitable<void> DatabaseHandler::HandleClassedEvent(std::shared_ptr<LogPasClientResponse> event)
{
	try {

		const std::string& login =
			event->GetEventMessage()->auth_data().login_response().login();

		const std::string& password =
			event->GetEventMessage()->auth_data().login_response().password();

		// Создаем запрос
		SQLite::Statement query{*db_read_write_, "SELECT uuid, password FROM user WHERE login = ?"};

		int uuid_column = 0;
		int password_column = 1;

		// Привязываем логин к первому (и единственному) параметру запроса
		query.bind(1, login);

		// Выполняем запрос
		if (!query.executeStep())
		{
			logger::ErrorSqlExecuteStep(event->GetType());
			event_queue_.AddEvent(std::make_shared<FailureCheckLogPas>(event->GetSessionId())); // !!! ошибка, а не failure
			co_return;
		}
		// Получаем пароль из результата
		const std::string& stored_password = query.getColumn(password_column).getString();
		std::shared_ptr<const std::string> stored_uuid = 
			std::make_shared<const std::string>(query.getColumn(uuid_column).getString());
	
		
		// Проверяем соответствие пароля
		if (password == stored_password)
		{
			event_queue_.AddEvent(std::make_shared<SuccessCheckLogPas>(event->GetSessionId(), stored_uuid));
		}
		else
		{
			event_queue_.AddEvent(std::make_shared<FailureCheckLogPas>(event->GetSessionId()));
		}
	}
	catch (const std::exception& e)
	{
		MVP_LOG_EXEPTION(e);
	}

	co_return;
}

template<>
boost::asio::awaitable<void> DatabaseHandler::HandleClassedEvent(std::shared_ptr<AvatarListClientRequest> event)
{
	try
	{
		// Создаем запрос
		SQLite::Statement query{*db_read_write_, "SELECT id, name FROM avatar WHERE user_uuid = ?"};

		int id_column = 0;
		int name_column = 1;
		
		// Привязываем логин к первому (и единственному) параметру запроса
		query.bind(1, event->GetUuid());

		using pair = std::pair<int32_t, std::string>;
		std::shared_ptr<std::vector<pair>> avatars =
			std::make_shared<std::vector<pair>>();

		while (query.executeStep())
		{
			avatars->push_back(pair{query.getColumn(id_column).getInt(), query.getColumn(name_column).getText()});
		}

		event_queue_.AddEvent(std::make_shared<AvatarListIsReady>(avatars, event->GetSessionId()));
	}
	catch (const std::exception& e)
	{
		MVP_LOG_EXEPTION(e);
	}

	co_return;
}

template<>
boost::asio::awaitable<void> DatabaseHandler::HandleClassedEvent(std::shared_ptr<GetAvatarsData> event)
{
	try
	{
		SQLite::Statement query{*db_read_write_, "SELECT * FROM avatar WHERE id = ?"};

		query.bind(1, event->GetIdSelectedAvatar());

		if (!query.executeStep())
		{
			logger::ErrorSqlExecuteStep(event->GetType());			
			co_return;
		}
		
		enum cid
		{
			id = 0
			, user_uuid
			, name
			, race_id
			, class_id
			, level
			, experience
			, health
			, mana
			, strength
			, dexterity
			, intelligence
			, location_id
			, pos_x
			, pos_y
		};

		std::shared_ptr<Avatar> avatar_ptr = std::make_shared<Avatar>(
			query.getColumn(cid::id).getInt()
			, std::move(query.getColumn(cid::user_uuid).getString()));

		avatar_ptr->SetAllNotConstParametrs(std::move(query.getColumn(cid::name).getString())
			, query.getColumn(cid::race_id).getInt()
			, query.getColumn(cid::class_id).getInt()
			, query.getColumn(cid::level).getInt()
			, query.getColumn(cid::experience).getInt()
			, query.getColumn(cid::health).getInt()
			, query.getColumn(cid::mana).getInt()
			, query.getColumn(cid::strength).getInt()
			, query.getColumn(cid::dexterity).getInt()
			, query.getColumn(cid::intelligence).getInt()
			, query.getColumn(cid::location_id).getInt()
			, query.getColumn(cid::pos_x).getInt()
			, query.getColumn(cid::pos_y).getInt()
		);

		event_queue_.AddEvent(std::make_shared<AvatarsDataReady>(event->GetSessionId(), std::move(avatar_ptr)));
	}
	catch (const std::exception& e)
	{
		MVP_LOG_EXEPTION(e);
	}

	co_return;
}

template<>
boost::asio::awaitable<void> DatabaseHandler::HandleClassedEvent(std::shared_ptr<AddItemInInventory> event)
{
	try
	{	
		SQLite::Statement query_add_item(*db_read_write_, "INSERT INTO inventory (avatar_id, item_id) VALUES (?, ?)");

		query_add_item.bind(1, event->GetAvatarId());
		query_add_item.bind(2, static_cast<uint32_t>(event->GetItem()));
		query_add_item.exec();
		query_add_item.reset();
	}
	catch (const std::exception& e)
	{
		MVP_LOG_EXEPTION(e);
	}

	co_return;
}


template<>
boost::asio::awaitable<void> DatabaseHandler::HandleClassedEvent(std::shared_ptr<SaveGame> event)
{
	try
	{
		const Avatar & avatar = *event->GetConstAvatar();

		enum qp
		{
			level = 1
			, experience
			, health
			, mana
			, strength
			, dexterity
			, intelligence
			, location_id
			, pos_x
			, pos_y
			, avatar_id
		};	

		SQLite::Statement query{*db_read_write_, "UPDATE avatar SET level = ?"
			", experience = ?, health = ?, mana = ?, strength = ?, dexterity = ?"
			", intelligence = ?, location_id = ?, pos_x = ?, pos_y = ?"
			"WHERE id = ?"};

		query.bind(qp::level, avatar.GetLevel());
		query.bind(qp::experience, avatar.GetExperience());
		query.bind(qp::health, avatar.GetHealth());
		query.bind(qp::mana, avatar.GetMana());
		query.bind(qp::strength, avatar.GetStrength());
		query.bind(qp::dexterity, avatar.GetDexterity());
		query.bind(qp::intelligence, avatar.GetIntelligence());
		query.bind(qp::location_id, avatar.GetLocationId());
		query.bind(qp::pos_x, avatar.GetPosition().first);
		query.bind(qp::pos_y, avatar.GetPosition().second);
		query.bind(qp::avatar_id, avatar.GetId());

		query.exec();
		query.reset();
		
		if (event->IsExitGame())
		{
			event_queue_.AddEvent(std::make_shared<ExitGameResponse>(event->GetSessionId()));
		}
		else
		{
			/*TODO: new event "game is saved"*/
		}
	}
	catch (const std::exception& e)
	{
		MVP_LOG_EXEPTION(e);
	}

	co_return;
}
