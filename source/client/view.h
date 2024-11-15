#pragma once
#include <string>
#include <mutex>
#include <iostream>

#include "messages.pb.h"
#include "settings.h"
#include "game_elements.h"
#include "menus.h"
#include "logger.h"
#include "texts.h"

class View
{
public:
	View();
	~View();

	void ShowTextWithNewLine(const std::string& s);
	void ShowTextWithoutNewLine(const std::string& s);
	void ShowErrorInput();
	void ShowErrorSendMessageToServer();

	/*It uses ProtoMessage here so to lock mutex only one time.*/
	void ShowAvatarList(const ProtoMessage::GameMessage& message);
	void ShowErrorChoiseAvatar();
	void ShowErrorStartGame();
	void ShowStartGameMessage(const Avatar&, const Location&, std::mutex&);
	void ShowCurrentState(const Avatar&, const Location&, std::mutex&, bool cout_lock = false);
	/*void ShowMenu(Menu&);*/

	template<typename T>
	void ShowMenu(Menu<T>& menu);

	void ShowMap(const Avatar&, const Location&, std::mutex&);
	void ShowLocDescription(const Location&, std::mutex&);
	void ShowCommonMessage(const std::string& name, const std::string& text);

private:
	std::mutex cout_mutex_;
	std::ostream& view_out_;

};

template<typename T>
void View::ShowMenu(Menu<T>& menu)
{
try
{
	using namespace text_v;	

	for (const auto& item : menu.GetMap())
	{
		view_out_ << kNewStr << (static_cast<uint16_t>(item.first))
			<< kColon << item.second;
	}
}
catch (std::exception& e)
{
	MVP_LOG_EXEPTION(e)
}
}/* ShowMenu */



