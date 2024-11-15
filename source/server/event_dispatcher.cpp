#include "event_dispatcher.h"
//

void EventDispatcher::RegisterHandler(const EventType type, 
    std::shared_ptr<EventHandler> handler, const std::string& event_routing_key)
{
    client_handlers_[event_routing_key][type].push_back(handler);
}

void EventDispatcher::UnregisterHandlers(const std::string& event_routing_key)
{
    client_handlers_.erase(event_routing_key);
}

boost::asio::awaitable<void> EventDispatcher::Dispatch(EventQueue& event_queue)
{
    auto executor = co_await boost::asio::this_coro::executor;

    for (;;) 
    {
    try
    {
        // Проверяем наличие события в очереди
        
       /* {
            logger::ErrorEmptyQueue(); // !!!
        }*/    
        if (!event_queue.IsEmpty())
        {
            shrd_event event = event_queue.ExtractTop();
            EventType type = event->GetType();

            const std::string& event_routing_key = event->GetRoutingKey();
                       
            if (type == EventType::kBroadcastMessage) {

                for (auto& [client_id, handlers_map] : client_handlers_)
                {
                    if (client_id != event_routing_key && client_id != settings::server::kDatabaseEventRoutingKey)
                    {
                        for (auto& event_handler : handlers_map[type]) {
                            boost::asio::co_spawn(executor, event_handler->ForwardEventToHandler(event), boost::asio::detached);
                        }
                    }
                }
                continue;
            }

            if (type == EventType::kDisconnectSession)
            {
                client_handlers_.erase(event_routing_key);
                continue;
            }
            
            // Находим соответствующие обработчики для данного типа события
            if (client_handlers_.find(event_routing_key) == client_handlers_.end()
             || client_handlers_[event_routing_key].find(type) == client_handlers_[event_routing_key].end())
            {
                logger::ErrorHandlerNotFound(type); //!!!
                continue;
            }

            // !!! TODO: если обработчика нет, удалить из очереди и показать ошибку

            for (auto& event_handler : client_handlers_[event_routing_key][type])
            {
                boost::asio::co_spawn(executor, event_handler->ForwardEventToHandler(event), boost::asio::detached);           
            }
            
        }

        // Планируем следующий вызов через небольшую задержку
        boost::asio::steady_timer timer(executor, std::chrono::milliseconds(50)); // !!!
        co_await timer.async_wait(boost::asio::use_awaitable);
    }
    catch (const std::exception& e)
    {
        MVP_LOG_EXEPTION(e)
    }
    }

}

