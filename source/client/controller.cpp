#include "controller.h"
#include "logger.h"

using proto_message = ProtoMessage::GameMessage;
using const_proto_ptr = std::unique_ptr<const proto_message>;

Controller::Controller(View& v, DatabaseHandler& m)
    : view_{v}, net_handler_{}, game_logic_{v, m, *this, *this}
{
}

Controller::~Controller()
{
    
}

void Controller::Start()
{

try
{
    auto send_message_to_model = [this](const proto_message& message)
        {
            game_logic_.SelectFunction(message);
        };

    auto send_message_to_net_handler = [this](const_proto_ptr message)
        {
            net_handler_.Send(std::move(message));
        };

    net_handler_.RegisterSingleObserver(send_message_to_model);
    game_logic_.RegisterSingleObserver(send_message_to_net_handler);

    net_handler_.ConnectToServer();
    net_handler_.StartListening();
}
catch (std::exception& e)
{
    MVP_LOG_EXEPTION(e)
} 
}/* Start */

void Controller::StopGame()
{
    net_handler_.StopListening(); 
}



std::string Controller::GetString(std::function<void()> original_req)
{
    bool correct_input{false};
    std::string str{};

    try
    {
        while (!correct_input)
        {
            std::getline(std::cin, str);

            if (std::cin.fail()
                || str.empty()
                || std::all_of(str.begin(), str.end(), isspace))
            {
                view_.ShowErrorInput();
                original_req();
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }

            correct_input = true;
        }
    }
    catch (std::exception& e)
    {
        MVP_LOG_EXEPTION(e)
    }
    return str; /* NRVO */

}/* GetString */


uint16_t Controller::GetInt(std::function<void()> original_req, uint16_t max_number)
{
    bool correct_input{false};
    uint16_t number{};

    try
    {
        while (!correct_input)
        {
            std::cin >> number;

            if (std::cin.fail() || number > max_number)
            {
                view_.ShowErrorInput();
                original_req();
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }

            correct_input = true;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    catch (std::exception& e)
    {
        MVP_LOG_EXEPTION(e)
    }
    return number;

}/* GetInt */

