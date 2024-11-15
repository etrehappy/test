#include "view.h"
#include "logger.h"
#include "texts.h"

using namespace text_v;

View::View()
    :cout_mutex_{}, view_out_{std::cout}
{
    
}

View::~View()
{
}

void View::ShowTextWithNewLine(const std::string& s)
{
try
{
    std::lock_guard<std::mutex> g{cout_mutex_};

    view_out_ << kNewStr << s;
}
catch (std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}
}/* ShowTextWithNewLine */

void View::ShowTextWithoutNewLine(const std::string& s)
{
try
{
    std::lock_guard<std::mutex> g{cout_mutex_};

    view_out_ << s;
}
catch (std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}
}/* ShowTextWithoutNewLine */

void View::ShowErrorInput()
{
try
{
    std::lock_guard<std::mutex> g{cout_mutex_};
    system("cls");
    view_out_ << kNewStr << kErrorInput;
}
catch (std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}
}/* ShowErrorInput */


void View::ShowErrorSendMessageToServer()
{
try
{
    std::lock_guard<std::mutex> g{cout_mutex_};
    system("cls");
    view_out_ << kNewStr << kErrorendMessageToServer;
}
catch (std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}
}/* ShowErrorSendMessageToServer */


void View::ShowAvatarList(const ProtoMessage::GameMessage& message)
{
try
{
    std::lock_guard<std::mutex> g{cout_mutex_};

    int it = 1;
    for (const auto& avatar : message.avatar_list_data().avatar_list())
    {
        view_out_ << kNewStr << it << ") " << avatar.name();

        ++it;
    }
}
catch (std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}
}/* ShowAvatarList */


void View::ShowErrorChoiseAvatar()
{
try
{
    std::lock_guard<std::mutex> g{cout_mutex_};

    view_out_ << kNewStr << kErrorChoiseAvatar;
}
catch (std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}
}/* ShowErrorChoiseAvatar */

void View::ShowErrorStartGame()
{
try
{
    std::lock_guard<std::mutex> g{cout_mutex_};
    system("cls");
    view_out_ << kNewStr << kErrorStartGame;
}
catch (std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}
}/* ShowErrorStartGame */

void View::ShowStartGameMessage(const Avatar& a, const Location& l, std::mutex& m)
{
try
{
    std::lock_guard<std::mutex> g{cout_mutex_};
    system("cls");
    view_out_ << kNewStr << kStartGame;
    ShowCurrentState(a, l, m, true);
}
catch (std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}
}/* ShowStartGameMessage */

void View::ShowCurrentState(const Avatar& avatar, const Location& location
    , std::mutex& avatar_and_location, bool cout_lock)
{
try
{    
    std::unique_lock<std::mutex> guard_cout{cout_mutex_, std::defer_lock};
          
    if (!cout_lock) 
    { 
        guard_cout.lock();
    }
    
    std::lock_guard<std::mutex> guard_game_data{avatar_and_location};

    view_out_ << kNewStr << text_v::kAvatarState
        << text_v::kLvl << avatar.GetLevel()
        << kSpace << text_v::kHealth << avatar.GetHealth()
        << kSpace << text_v::kMana << avatar.GetMana()
        << kNewStr << text_v::kCurrentLocation << location.GetName() << std::endl;
}
catch (std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}
}/* ShowCurrentState */


void View::ShowMap(const Avatar& a, const Location& l, std::mutex& avatar_and_location)
{
try
{
    std::scoped_lock lock(cout_mutex_, avatar_and_location);
    
    /* TODO: Make a more universal function */

    view_out_ << "Карта таверны:\n";

    for (const auto& line : l.GetMap()) 
    {
        view_out_ << line << std::endl;
    }

    view_out_ << "\nЛегенда символов:\n"
        << map_symbols::kWall << " - Стена\n"
        << map_symbols::kDoor << " - Дверь\n"
        << map_symbols::kNpcLora << " - Хозяйка Лора\n"
        << map_symbols::kNpcAdrik << " - Помощник Адрик\n"
        << map_symbols::kPlayer << " - Игрок\n" << std::endl;
}
catch (std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}

}


void View::ShowLocDescription(const Location& l, std::mutex& avatar_and_location)
{
try
{
    std::scoped_lock lock(cout_mutex_, avatar_and_location);

    /* TODO: Make a more universal function */

    view_out_ << "Я нахожусь в " << l.GetName() << ". " << l.GetDescription() << std::endl;
}
catch (std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}

}

void View::ShowCommonMessage(const std::string& name, const std::string& text)
{
try
{
    std::lock_guard<std::mutex> g{cout_mutex_};

    view_out_ << "[Чат мир]::" << name << ": " << text;
    view_out_ << kNewStr << kBracket;
}
catch (std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}
}







