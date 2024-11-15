#include "parser.h"
#include "logger.h"
#include "texts.h"

namespace b_po = boost::program_options;
using namespace text_p;

Parser::Parser()
    :desc_{kOptDescription}, pos_desc_{}, args_t_{}, arg_t_{}
{
    Init();
}

Parser::~Parser()
{
}

void Parser::Init()
{

try
{    
    desc_.add_options()
        (kCommandKey, b_po::value<std::string>(), kCommandKeyDescription)
        (kArgsKey, b_po::value<std::vector<std::string>>()->multitoken(), kArgsKeyDescription);       

    int first_position = 1;
    int second_pos_unlimited = -1;

    pos_desc_.add(kCommandKey, first_position);
    pos_desc_.add(kArgsKey, second_pos_unlimited);
}
catch (std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}    
} /* Init */


/* thread StartGame */
CommandVariant Parser::GetUserCommand(std::string& user_input)
{
try
{   
    b_po::variables_map vm;
    ParseAndNotify(vm, user_input);   

    return ConvertToEnum(vm);
}
catch (std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}
    return CommandList::kNone;

} /* HandleCommandVariant */


/* thread StartGame */
void Parser::ParseAndNotify(b_po::variables_map& vm, std::string& command_str)
{

try
{
    args_t_.clear();
    arg_t_.clear();
    std::istringstream iss{command_str};

    while (iss >> arg_t_)
    {
        args_t_.push_back(arg_t_);
    }
        
    b_po::store(b_po::command_line_parser(args_t_).options(desc_).positional(pos_desc_).run(), vm);
    b_po::notify(vm);
}
catch (std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}
} /* ParseAndNotify */


/* thread StartGame */
CommandVariant Parser::ConvertToEnum(b_po::variables_map& vm)
{    
try
{   
    if (!vm.count(kCommandKey))
    {
        /*пустое !!!*/
        return CommandList::kNone;
    }

    std::string command = vm[kCommandKey].as<std::string>();

    if (command == command_str::kMenu)          { return CommandList::kMenu; }
    else if (command == command_str::kSpeak)    { return CommandList::kSpeak; }
    else if (command == command_str::kChat)     { return std::move(ParseChatMessage(vm)); }
    else if (command == command_str::kMap) { return CommandList::kMap; }
    else if (command == command_str::kLocation) { return CommandList::kCurrentLocation; }
    else if (command == command_str::kGo)       { return std::move(ParseGo(vm)); }
    else
    {
        return CommandList::kNone;
    }

} /* try */
catch (std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}/* catch */
    return CommandList::kNone;

}/* ConvertToEnum */

CommandVariant Parser::ParseChatMessage(b_po::variables_map& vm)
{
    using pair = std::pair<CommandList, ChatMessage>;
try
{
    if (!vm.count(kArgsKey))
    {
        /*пустое !!!*/
        return CommandList::kNone;
    }

    auto args = vm[kArgsKey].as<std::vector<std::string>>();

    if (args.size() < command_str::kChatMinArgsSize)
    {
        /*ошибка !!!*/
        return CommandList::kNone;
    }
    ChatMessage сhat_message{args[0], args[1]};

    for (size_t i = 2; i < args.size(); ++i)
    {
        сhat_message.text += " " + args[i];
    }   

     return std::make_unique<pair>(CommandList::kChat, std::move(сhat_message));

} /* try */
catch (std::exception& e)
{
    MVP_LOG_EXEPTION(e)

}/* catch */

    return CommandList::kNone;

}/* ParseChatMessage */


CommandVariant Parser::ParseGo(b_po::variables_map& vm)
{
    using pair = std::pair<CommandList, GoCommand>;
try
{
    if (!vm.count(kArgsKey))
    {
        /*пустое !!!*/
        return CommandList::kNone;
    }

    auto args = vm[kArgsKey].as<std::vector<std::string>>();

    if (args.size() < command_str::kGoMinArgsSize)
    {
        /*ошибка !!!*/
        return CommandList::kNone;
    }

    GoCommand go_command{};
    std::string direction = args[0];
    go_command.steps = std::stoi(args[1]);

    if (direction == command_str::kNorth || direction == command_str::kN)
    {   
        go_command.direction = DirectionsList::kNorth;
        return std::make_unique<pair>(CommandList::kGo, go_command);
    }
    else if (direction == command_str::kSouth || direction == command_str::kS)
    {
        go_command.direction = DirectionsList::kSouth;
        return std::make_unique<pair>(CommandList::kGo, go_command);
    }
    else if (direction == command_str::kWest || direction == command_str::kW)
    {
        go_command.direction = DirectionsList::kWest;
        return std::make_unique<pair>(CommandList::kGo, go_command);
    }
    else if (direction == command_str::kEast || direction == command_str::kE)
    {
        go_command.direction = DirectionsList::kEast;
        return std::make_unique<pair>(CommandList::kGo, go_command);
    }
    else
    {
        /*TODO: error*/
        return CommandList::kNone;
    }

} /* try */
catch (std::exception& e)
{
    MVP_LOG_EXEPTION(e)

}/* catch */

    return CommandList::kNone;

}/* ParseChatMessage */

