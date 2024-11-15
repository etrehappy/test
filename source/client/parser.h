#pragma once

#include <boost/program_options.hpp>
#include <string>
#include <variant>
#include <memory>

enum class CommandList : uint32_t
{
	kNone = 0
	
	, kMenu
	, kSpeak
	, kChat
	, kMap
	, kCurrentLocation
	, kGo
	
	, kEnd
};

enum class DirectionsList : uint16_t
{
	kNone = 0

	, kNorth
	, kSouth
	, kWest
	, kEast

	, kEnd
};

struct ChatMessage
{
	std::string recipient{};
	std::string text{};
};

struct GoCommand
{
	DirectionsList direction{DirectionsList::kNone};
	int32_t steps{0};
};

using CommandVariant = std::variant<CommandList
	, std::unique_ptr<std::pair<CommandList, ChatMessage>>
	, std::unique_ptr<std::pair<CommandList, GoCommand>>
>;


class Parser
{
public:
	/*namespace boost::program_options = boost::program_options;*/

	Parser();
	~Parser();


	CommandVariant GetUserCommand(std::string&);

private:
		
	void Init();
	void ParseAndNotify(boost::program_options::variables_map&, std::string& );
	CommandVariant ConvertToEnum(boost::program_options::variables_map&);
	CommandVariant ParseChatMessage(boost::program_options::variables_map&);
	CommandVariant ParseGo(boost::program_options::variables_map&);

	boost::program_options::options_description desc_;
	boost::program_options::positional_options_description pos_desc_;

	std::vector<std::string> args_t_;
	std::string arg_t_;
};