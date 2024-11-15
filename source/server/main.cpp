#include "main.h"


boost::asio::awaitable<void> ListenConnect(EventQueue& q, EventDispatcher& d)
{
    auto executor = co_await boost::asio::this_coro::executor;

    b_tcp_acceptor acceptor(executor, {tcp::v4(), settings::server::kPort});

    for (;;)
    {
        auto socket = std::make_shared<b_tcp_socket>(co_await acceptor.async_accept());
        
        try
        {
            auto session = std::make_shared<ClientSession>(socket, q, d);
            boost::asio::co_spawn(executor, session->Start(), boost::asio::detached);
        }
        catch (const std::exception& e)
        {
            MVP_LOG_EXEPTION(e)
        }
        
        
    }

}

int main() 
{
    std::locale::global(std::locale("ru_RU.utf8"));

    logger::InitLogging();

    try
    {
        boost::asio::io_context io_context{};

        // Создаем очередь событий
        EventQueue event_queue{io_context};

        // Создаем диспетчер событий
        EventDispatcher dispatcher{};

        Database database{};
       /* DatabaseHandler db_handler{database, event_queue};
        db_handler.RegisterInDispatcher(dispatcher);*/


        auto db_handler = std::make_shared<DatabaseHandler>(database, event_queue);

        // Регистрация обработчиков
        db_handler->RegisterInDispatcher(dispatcher);

        // Запускаем диспетчеризацию событий (асинхронно, без отдельного потока)
        boost::asio::co_spawn(io_context, dispatcher.Dispatch(event_queue), boost::asio::detached);

        boost::asio::co_spawn(io_context, ListenConnect(event_queue, dispatcher), boost::asio::detached);

        logger::ServerStart();

        io_context.run();
    }    
    catch (const std::exception& e) 
    {
        MVP_LOG_EXEPTION(e)
    }   

    return 0;
}


