#pragma once

#include <boost/asio.hpp>
#include <thread>
#include <mutex>

#include "messages.pb.h"

using b_tcp = boost::asio::ip::tcp;

class NetworkHandler
{
public:
	using proto_message = ProtoMessage::GameMessage;
	using const_proto_ptr = std::unique_ptr<const proto_message>;

	NetworkHandler();
	~NetworkHandler();

	void ConnectToServer();
	void StartListening();
	void StopListening();

	/*!
	* @brief GameLogic observes a new message from a server
	*/
	void RegisterSingleObserver(std::function<void(const proto_message&)>);

	void Send(const_proto_ptr);
private:
	/*!
	* @brief NetworkHandler notifies GameLogic about a new message from a server.
	*/
	void Listen();

	/*!
	* @brief MVP-version there isn't others functions in a main thread, so this function just join Listen().
	*/
	void NextFunction();

	boost::asio::io_context io_context_;
	b_tcp::socket socket_;
	b_tcp::resolver resolver_;

	std::atomic<bool> stop_listen_;
	std::thread listen_thread_;	
	std::function<void(const proto_message&)> notify_;


};


