#pragma once

#include <string>
#include <memory>

#include "game_logic.h"
#include "view.h"
#include "network_handler.h"
#include "db_handler.h"
#include "i_inputdata.h"

class Controller : public IInputDataProvider, IExit
{
public:
	Controller(View&, DatabaseHandler&);
	~Controller();

	/*!
	* @brief Controller observes message from NetworkHandler::Listen
	*/
	void Start();
	void StopGame() override;

	std::string GetString(std::function<void()>) override;
	uint16_t GetInt(std::function<void()>, uint16_t) override;

private:

	View& view_;
	NetworkHandler net_handler_;
	GameLogic game_logic_;
};

