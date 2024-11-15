
using b_await_void = boost::asio::awaitable<void>;

template<class T>
b_await_void EventHandler::PassClassedEvent(
	std::shared_ptr<Event> event,
	std::function<b_await_void(std::shared_ptr<T>)> handler_func,
	EventType type)
{
	auto executor = co_await boost::asio::this_coro::executor;

	auto classed_event = std::dynamic_pointer_cast<T>(event);

	if (!classed_event)
	{ 
		logger::ErrorDynamicPointerCast(type);
		co_return;
	}

	co_await handler_func(classed_event);

	co_return;
}

//template<typename T>
//boost::asio::awaitable<void> EventHandler::PassClassedEvent(
//	std::shared_ptr<Event> event, EventType type)
//{
//	auto executor = co_await boost::asio::this_coro::executor;
//	auto classed_event = std::dynamic_pointer_cast<T>(event);
//	if (!classed_event) {
//		logger::ErrorDynamicPointerCast(type);
//		co_return;
//	}
//
//	/*co_await HandleSpecificEvent<T>(classed_event);*/
//	boost::asio::co_spawn(executor, HandleSpecificEvent<T>(classed_event), boost::asio::detached);
//	co_return;
//}
//
//template<typename T>
//boost::asio::awaitable<void> EventHandler::HandleSpecificEvent(std::shared_ptr<Event> event) {
//	// В классе UserHandler можно перегружать этот метод для обработки разных типов событий.
//	co_return;
//}

//template<typename T, typename F>
//boost::asio::awaitable<void> PassClassedEvent(std::shared_ptr<Event>,
//	std::function<F(std::shared_ptr<T>)>, EventType);
//template<typename T>
//boost::asio::awaitable<void> /*EventHandler::*/PassClassedEvent(
//	std::shared_ptr<Event> event, 
//	std::function<boost::asio::awaitable<void>(std::shared_ptr<T>)> handler_func,
//	EventType type)
//{
//	auto executor = co_await boost::asio::this_coro::executor;
//
//	auto classed_event = std::dynamic_pointer_cast<T>(event);
//	if (!classed_event)
//	{ // если есть ошибка
//		logger::ErrorDynamicPointerCast(type);
//		co_return;
//	}	
//	
//	co_await handler_func(classed_event);
//}
//
//	case EventType::kSuccessCheckLogPas:
//		boost::asio::co_spawn(executor,
//			EventHandler::PassClassedEvent<SuccessCheckLogPas>(event,
//				ConfirmAuthenticate,
//				EventType::kSuccessCheckLogPas),
//			boost::asio::detached);
//		break;
//
//	case EventType::kFailureCheckLogPas:
//		boost::asio::co_spawn(
//			executor,
//			EventHandler::PassClassedEvent<FailureCheckLogPas>(event,
//				RejectAuthenticate,
//				EventType::kFailureCheckLogPas),
//			boost::asio::detached);
//		break;