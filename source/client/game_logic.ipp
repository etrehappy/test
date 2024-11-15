#include "game_logic.h"
#include "logger.h"
#include "texts.h"

using proto_message = ProtoMessage::GameMessage;
using const_proto_ptr = std::unique_ptr<const proto_message>;
using proto_ptr = std::unique_ptr<proto_message>;
using command_type = ProtoMessage::CommandType;
using chat_pair_ptr = std::unique_ptr<std::pair<CommandList, ChatMessage>>;
using go_pair_ptr = std::unique_ptr<std::pair<CommandList, GoCommand>>;

template<>
const_proto_ptr GameLogic::PrepareMessage<command_type::kLoginPasResponse>
	(const std::string& login, const std::string& password)
{
try
{	
	proto_ptr message = std::make_unique<proto_message>();
	message->set_command(command_type::kLoginPasResponse);
	message->mutable_auth_data()->mutable_login_response()->set_login(login);
	message->mutable_auth_data()->mutable_login_response()->set_password(password);

	return message;
}
catch (std::exception& e)
{
	MVP_LOG_EXEPTION(e)	
}
	return nullptr;
}

template<>
const_proto_ptr GameLogic::PrepareMessage<command_type::kUsersAvatarListClientRequest>()
{
try
{
	proto_ptr message = std::make_unique<proto_message>();
	message->set_command(command_type::kUsersAvatarListClientRequest);

	return message;
}
catch (std::exception& e)
{
	MVP_LOG_EXEPTION(e)	
}
	return nullptr;
}

template<>
const_proto_ptr GameLogic::PrepareMessage<command_type::kStartGameClientRequest>
	(const uint32_t& avatar_id)
{
try
{
	proto_ptr message = std::make_unique<proto_message>();
	message->set_command(command_type::kStartGameClientRequest);
	message->mutable_start_game()->set_id_selected_avatar(avatar_id);

	return message;
}
catch (std::exception& e)
{
	MVP_LOG_EXEPTION(e)
}
	return nullptr;
}

template<>
const_proto_ptr GameLogic::PrepareMessage<command_type::kExitGameClientRequest>()
{
try
{
    proto_ptr message = std::make_unique<proto_message>();
    message->set_command(command_type::kExitGameClientRequest);

    auto message_avatar = message->mutable_exit_game()->mutable_avatar();

    message_avatar->set_level(current_avatar_->GetLevel());
    message_avatar->set_experience(current_avatar_->GetExperience());
    message_avatar->set_health(current_avatar_->GetHealth());
    message_avatar->set_mana(current_avatar_->GetMana());
    message_avatar->set_strength(current_avatar_->GetStrength());
    message_avatar->set_dexterity(current_avatar_->GetDexterity());
    message_avatar->set_intelligence(current_avatar_->GetIntelligence());
    message_avatar->set_location_id(current_avatar_->GetLocationId());
    message_avatar->set_pos_x(current_avatar_->GetPosition().first);
    message_avatar->set_pos_y(current_avatar_->GetPosition().second);

    return message;
}
catch (std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}
return nullptr;
}

template<>
const_proto_ptr GameLogic::PrepareMessage<command_type::kSendToCommonChat>
    (const std::string& text_message)
{
try
{
    proto_ptr message = std::make_unique<proto_message>();

    message->set_command(ProtoMessage::CommandType::kSendToCommonChat);
    message->mutable_chat_data()->set_text_message(text_message);

    return message;
}
catch (std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}
    return nullptr;
}



template<>
void GameLogic::HandleCommand<CommandList::kMenu>()
{
try
{
    auto if_input_error = [this]() {
        view_.ShowTextWithNewLine(text_v::kMenuNumber);
        };
    const uint16_t offset = 1;
    const uint16_t max_number = static_cast<uint16_t>(EMainMenu::kEnd) - offset;

    view_.ShowMenu(main_menu_);
    view_.ShowTextWithNewLine(text_v::kMenuNumber);

    EMainMenu user_input = static_cast<EMainMenu>(
        data_provider_.GetInt(if_input_error, max_number));

    switch (user_input)
    {
    case EMainMenu::kSaveAndExit:            
        NotifyNewMessage(std::move(
            PrepareMessage<command_type::kExitGameClientRequest>()));
        stop_get_command_ = true;
        break;
    case EMainMenu::kEnd:
        break;
    default:
        break;
    }
}
catch (std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}
}

template<>
void GameLogic::HandleCommand<CommandList::kMap>()
{
try
{
    current_location_->UpdateSymbolsOnMap(*current_avatar_);
    view_.ShowMap(*current_avatar_, *current_location_, avatar_and_location_);
}
catch (std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}
}

template<>
void GameLogic::HandleCommand<CommandList::kCurrentLocation>()
{
try
{
    view_.ShowLocDescription(*current_location_, avatar_and_location_);
}
catch (std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}
}