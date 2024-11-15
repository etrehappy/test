
template<>
boost::asio::awaitable<void> UserHandler::HandleClassedEvent(std::shared_ptr<SuccessCheckLogPas> event)
{
	logged_in_ = true;
	uuid_ = event->GetUuid();

	co_return;
}
//
//template<>
//void NetworkHandler::HandleClassedEvent(std::shared_ptr<FailureCheckLogPas> event)
//{
//}

	//template<typename T>
	//void HandleClassedEvent(std::shared_ptr<T> event);


	//case EventType::kFailureCheckLogPas:
	//{
	//	boost::asio::co_spawn(
	//		executor,
	//		PassClassedEvent<FailureCheckLogPas>(
	//			event,
	//			std::bind(&NetworkHandler::HandleClassedEvent<FailureCheckLogPas>, this, std::placeholders::_1),
	//			EventType::kFailureCheckLogPas
	//		),
	//		boost::asio::detached);
	//}