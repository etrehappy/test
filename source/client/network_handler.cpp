#include "network_handler.h"
#include "settings.h"
#include "logger.h"

using proto_message = ProtoMessage::GameMessage;
using const_proto_ptr = std::unique_ptr<const proto_message>;

NetworkHandler::NetworkHandler()
    :io_context_{}, socket_{io_context_}, resolver_{io_context_}
    , stop_listen_{false}, listen_thread_{}, notify_ {}
{
}

NetworkHandler::~NetworkHandler()
{
    if (listen_thread_.joinable())
    {
        listen_thread_.join();
    }
}


void NetworkHandler::ConnectToServer()
{

try
{
    boost::asio::connect(socket_, resolver_.resolve(settings::client::kTestHost
        , settings::client::kPortStr));
    logger::Connect();
}
catch (std::exception& e) 
{
    MVP_LOG_EXEPTION(e)
}    

} /* ConnectToServer */

void NetworkHandler::StartListening() 
{
try
{
    stop_listen_ = false;
    listen_thread_ = std::thread(&NetworkHandler::Listen, this);

    NextFunction();
}
catch (std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}
} /* StartListening */

void NetworkHandler::StopListening() 
{
    stop_listen_ = true;
    //if (listen_thread_.joinable()) 
    //{
    //    listen_thread_.join();
    //}
    socket_.close();
}

void NetworkHandler::Listen()
{
try
{
    char buffer[settings::kBufferSize];
    boost::system::error_code error{};

    if (!notify_)
    {        
        logger::ErrorMessageCallbackNotSet(__func__);
        return;
    }

    while (!stop_listen_)
    {
        size_t length = socket_.read_some(boost::asio::buffer(buffer), error);

        if (error == boost::asio::error::eof)
        {
            logger::ErrorReadSome(error.message());
            return;
        }
        else if (error)
        {
            logger::ErrorReadSome(error.message());
            return;
        }
        


        std::string received_data(buffer, length);

        ProtoMessage::GameMessage message{};

        if (!message.ParseFromString(received_data)) 
        {
            logger::ErrorParsingMessage();
            continue;
        }

        if (message.command() == ProtoMessage::CommandType::kExitGameServerResponse)
        {
            StopListening();
        }

        notify_(message);
       
    }
}
catch (std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}
}/* Listen */

void NetworkHandler::NextFunction()
{
    if (listen_thread_.joinable())
    {
        listen_thread_.join();
    }
}


void NetworkHandler::RegisterSingleObserver(
    std::function<void(const ProtoMessage::GameMessage&)> callback)
{
    notify_ = callback;

    if (!notify_) 
    {
        logger::ErrorMessageCallbackNotSet(__func__);
    }
}

void NetworkHandler::Send(const_proto_ptr message)
{
try
{
    boost::system::error_code error{};

    std::string serialized{};

    if(!message->SerializeToString(&serialized))
    {
        logger::ErrorSerializeToString();
        return;
    };

    socket_.write_some(boost::asio::buffer(serialized), error);

    if (error) 
    {       
        logger::ErrorSend(error.message());
    }
}
catch (std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}
} /* Send */