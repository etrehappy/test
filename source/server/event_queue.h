#pragma once

#include <queue>
#include <boost/asio.hpp>

#include "event.h"
#include "logger.h"

struct EventComparator 
{
    bool operator()(const shrd_event& a, const shrd_event& b)
    {
        return a->GetPriority() < b->GetPriority();
    }
};

//struct EventWithType {
//    EventType type;
//    shrd_event event;
//};

class EventQueue
{
public:
    EventQueue(boost::asio::io_context&);
    ~EventQueue();

    void AddEvent(shrd_event event);
    const bool IsEmpty() const;

    shrd_event ExtractTop();


private:
    boost::asio::io_context& io_context_;
    std::priority_queue<shrd_event, std::vector<shrd_event>, EventComparator> queue_;
    //std::priority_queue<EventWithType, std::vector<EventWithType>, EventComparator> queue_;    
};



//class EventQueue {
//public:
//    EventQueue(boost::asio::io_context& io_context)
//
//
//        // Добавление события в очередь
//        void push(std::shared_ptr<Event> event) {
//        queue_.push(event);
//    }
//
//    // Обработка всех событий в очереди
//    void processEvents() {
//        while (!queue_.empty()) {
//            auto event = queue_.front();
//            queue_.pop();
//            event->execute(); // Выполнение события
//        }
//    }
//
//    // Асинхронная обработка событий через Boost.Asio
//    void asyncProcess() {
//        io_context_.post([this]() {
//            processEvents(); // Обрабатываем все события в очереди
//            });
//    }
//
//private:
//
//};

