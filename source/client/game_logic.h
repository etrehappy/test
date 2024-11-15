#pragma once

#include <memory>
#include <mutex>
#include <thread>

#include "game_elements.h"
#include "messages.pb.h"
#include "db_handler.h"
#include "view.h"
#include "parser.h"
#include "i_inputdata.h"
#include "menus.h"

/* Macro is used in GetInt to clear std::cin */
//#define MAX_USER_INPUT_NUMBER_TYPE uint32_t

/*!
*	@brief This is a part of Controller. It job is to manage dfferent events in
*		the game (like an authorisation, an move, etc.).
*/
class GameLogic
{
public:
    using proto_message = ProtoMessage::GameMessage;
    using const_proto_ptr = std::unique_ptr<const proto_message>;
    using proto_ptr = std::unique_ptr<proto_message>;
    using chat_pair_ptr = std::unique_ptr<std::pair<CommandList, ChatMessage>>;
    using go_pair_ptr = std::unique_ptr<std::pair<CommandList, GoCommand>>;

    GameLogic(View&, DatabaseHandler&, IInputDataProvider&, IExit&);
    ~GameLogic();

    /*!
    *	@brief NetworkHandler observes a new message for a server
    */
    void RegisterSingleObserver(std::function<void(const_proto_ptr)>);

    void SelectFunction(const proto_message&);

private:

   
/* 1. Authentication */

    void SendLogPasResponse();
    std::pair<const std::string, const std::string> GetCredentials();
    void AuthSuccess();
    void AuthFailure();
    
/* 2. Сhoose an avatar */

    void HandlerChooseAvatar(const proto_message&);
    uint32_t GetChoisedAvatarId(const proto_message&);
    uint32_t FindAvatarId(uint16_t, const proto_message&);

    /*!
    *	@brief MVP-version hasn't a realisation of it.
    */
    void CreateAvatar();

/* 3. Start a game */

    void SetAvatar(const proto_message&);
    void HandlerStartGame(const proto_message&);
    void StartGame(); 
     
    void HandleCommandVariant(CommandList&);
    void HandleCommandVariant(chat_pair_ptr&);  
    void HandleCommandVariant(go_pair_ptr&);

    template <CommandList>
    void HandleCommand();

    void HandlerCommonMessage(const proto_message&);

    /*!
    *	@brief If steps > 0 - move to east, if < 0 - move to west.
    */
    void MoveX(int32_t steps);
    /*!
    *	@brief If steps > 0 - move to south, if < 0 - move to north.
    */
    void MoveY(int32_t steps);

/* Everything else */

    void StopGame();
    void NotifyNewMessage(const_proto_ptr );

    template<ProtoMessage::CommandType T, typename... Args>
    const_proto_ptr PrepareMessage(Args&&... args);

    std::string uuid_{};
    std::shared_ptr<Avatar> current_avatar_;
    std::shared_ptr<Location> current_location_;
    View& view_;
    DatabaseHandler& model_;
    IInputDataProvider& data_provider_;
    IExit& exit_; // !!!
    std::mutex avatar_and_location_{};

    std::function<void(const_proto_ptr)> net_notify_;
    std::thread game_thread_;
    const std::shared_ptr<Parser> parser_;
    bool stop_get_command_;

    MainMenu main_menu_;


};

