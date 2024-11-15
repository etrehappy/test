#pragma once

#include "event.h"

#include <functional>

#include <boost/asio.hpp>
#include <boost/asio/experimental/awaitable_operators.hpp>
#include <boost/asio/awaitable.hpp>

#include "logger.h"

class EventDispatcher;

class EventHandler
{
public:
	using b_await_void = boost::asio::awaitable<void>;

	virtual ~EventHandler() = default;
	
	virtual b_await_void ForwardEventToHandler(shrd_event) = 0; //!!! по ссылке?
	virtual void RegisterInDispatcher(EventDispatcher& d) = 0;

	/*!
	* @brief PassClassedEvent uses dynamic_pointer_cast to convert the event into
	*	a derived class and pass it to a handler-function. Event conversion is 
	*	used to get specific member functions of the event.
	*	For exapmle, event LogPasClientResponse.
	*/
	template<typename T>
	b_await_void PassClassedEvent(std::shared_ptr<Event>,
		std::function<b_await_void(std::shared_ptr<T>)>, EventType);


private:
};

#include "event_handler.ipp"


