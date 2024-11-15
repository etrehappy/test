#pragma once

#include <unordered_map>
#include <functional>
#include <memory>
#include <string>

#include <boost/asio.hpp>
#include <boost/asio/experimental/awaitable_operators.hpp>
#include <boost/asio/awaitable.hpp> 

#include "event.h"
#include "event_handler.h"
#include "event_queue.h"
#include "logger.h"

class EventDispatcher {
public:
    //using EventHandler = std::function<void(shrd_event)>;

    // Регистрация обработчика для определенного типа события
    void RegisterHandler(const EventType type, std::shared_ptr<EventHandler> handler, const std::string& event_routing_key);
    void UnregisterHandlers(const std::string& event_routing_key);

    // Диспетчеризация события (выполнение всех связанных обработчиков)
    boost::asio::awaitable<void> Dispatch(EventQueue& event_queue); // !!! ссылка на event?   


private:
    //std::unordered_map<EventType, std::vector<EventHandler>> handlers_; // Таблица обработчиков
   // std::unordered_map<EventType, std::vector<std::shared_ptr<EventHandler>>> handlers_; // Таблица обработчиков

    std::unordered_map<std::string, std::unordered_map<EventType, std::vector<std::shared_ptr<EventHandler>>>> client_handlers_;
};