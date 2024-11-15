#include "session.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>



ClientSession::ClientSession(std::shared_ptr<b_tcp_socket> s, EventQueue& q, EventDispatcher& d)
    : socket_{s}, event_queue_{q}, dispatcher_ {d}
    , client_ip_{socket_->remote_endpoint().address().to_string() 
    + ":" + std::to_string(socket_->remote_endpoint().port())}
    , session_id_{boost::uuids::to_string(boost::uuids::random_generator()())}
    , active_(true)

{       
    user_handler_ = std::make_shared<UserHandler>(q, session_id_);
    user_handler_->RegisterInDispatcher(dispatcher_);

    network_handler_ = std::make_shared<NetworkHandler>(s, q, session_id_);
    network_handler_->RegisterInDispatcher(dispatcher_);

    game_handler_ = std::make_shared<GameHandler>(q, session_id_);
    game_handler_->RegisterInDispatcher(dispatcher_);

    logger::ClientConnect(client_ip_);
}

boost::asio::awaitable<void> ClientSession::Start()
{    

    try {

        auto self = shared_from_this();

        event_queue_.AddEvent(std::make_shared<FirstConnect>(session_id_));

        co_await network_handler_->GetClientResponse(self);
    }
    catch (const std::exception& e) 
    {
        BOOST_LOG_TRIVIAL(error) << "Ошибка: " << e.what();
    }
}

bool ClientSession::IsActive()
{
    return active_;
}

void ClientSession::SetInactive()
{
    active_ = false;
    dispatcher_.UnregisterHandlers(session_id_);
    logger::ClientDisconnect(client_ip_);
}





//// Получаем IP-адрес клиента
//     
//

/////boost::asio::co_spawn(executor, HandleConnection(socket), boost::asio::detached);

