#pragma once

#include <string>
#include <memory>

#include "messages.pb.h"
#include "settings.h"
#include "game_elements.h"

enum class EventPriority : unsigned short
{
    kNone = 0

    /*
    * Events with thise priority are handled last.
    */
    //--

    /*
    * Events with these priority are handled the faster the high EventPriority.
    */
    , kLow
    , kMedium
    , kHigh

    /*
    * Events with thise priority are handled first.
    */
    , kError
    , kEndGame
};

enum class EventType : unsigned short
{
    kNone = 0
    
    /* UserHandler handles these events */
    , kFirstConnect
    , kSuccessCheckLogPas
    , kFailureCheckLogPas
    , kAvatarsDataReady    

    /* NetworkHandler handles these events */
    , kUserLogPasServerRequest
    , kAvatarListIsReady
    , kStartGameServerResponseSucsses 
    , kExitGameResponse
    
    /* DatabaseHandler handles these events */
    , kUserLogPasClientResponse
    , kUsersAvatarListClientRequest
    , kGetAvatarsData
    , kAddItemInInventory    
    , kSaveGame

    /* GameHandler handles these events */
    , kExitGameRequest

    /* Few handlers handle these events */
    , kStartGameClientRequest /*DatabaseHandler + GameHandler*/

    /* EventDispatcher routing these events */
    , kBroadcastMessage /* to the NetworkHandler */
    , kDisconnectSession /* to the session */ //!!!

    , kEnd
};




class Event 
{
public:

    Event(EventPriority, EventType, const std::string&); // !!! нужен ли shared
    virtual ~Event() = default;
    //virtual std::string GetType() const = 0;
    //virtual void Execute() = 0; // Метод для выполнения конкретных событий

    EventPriority GetPriority() const;
    EventType GetType() const;

    /*!
    * @brief event_routing_key_ == client's session_id_ for most events 
    *   except those handled by the DatabaseHandler, because in the MVP-version 
    *   the DatabaseHandler is one for all clients. 
    */
    const std::string& GetRoutingKey() const;

private:
    const EventPriority priority_; // !!!
    const EventType type_; // !!!
    //std::shared_ptr<ClientSession> session_;
    const std::string& event_routing_key_;
};


using shrd_event = std::shared_ptr<Event>; // !!!
//using shrd_event = std::unique_ptr<Event>;





/*!
* @brief Add these events: ClientSession.
*       Handles: 
*/
class FirstConnect : public Event
{
public:
    FirstConnect(const std::string&/*, std::shared_ptr<ClientSession> s*/);

private:
    
};

/*!
* @brief Add these events: UserHandler.
*       Handles:
*/
class LoginRequest : public Event
{
public:
    LoginRequest(const std::string&);

private:

};


/*!
* @brief Added: NetworkHandler.
*       Handles: DatabaseHandler
*/
class LogPasClientResponse : public Event
{
public:
    LogPasClientResponse(const std::shared_ptr<ProtoMessage::GameMessage>, const std::string& session_id);
    const std::shared_ptr<ProtoMessage::GameMessage> GetEventMessage() const;

    /*!
    * @brief Used by DatabaseHandler to add this event in the common queue. 
    */
    const std::string& GetSessionId();
private:
    const std::shared_ptr<ProtoMessage::GameMessage> message_;
    const std::string& session_id_;
}; 


/*!
* @brief Add these events: DatabaseHandler.
*       Handles: NetworkHandler in MVP-version, UserHandler
*/
class SuccessCheckLogPas : public Event
{
public:
    SuccessCheckLogPas(const std::string& session_id, std::shared_ptr<const std::string> uuid);
    std::shared_ptr<const std::string> GetUuid();
private:
    std::shared_ptr<const std::string> uuid_{};
};

/*!
* @brief Add these events: DatabaseHandler.
*       Handles: NetworkHandler in MVP-version
*/
class FailureCheckLogPas : public Event
{
public:
    FailureCheckLogPas(const std::string&);

private:

};

/*!
* @brief Added: NetworkHandler.
*       Handles: DatabaseHandler
*/
class AvatarListClientRequest : public Event
{
public:
    AvatarListClientRequest(const std::string& session_id, const std::string& uuid);

    /*!
    * @brief Used by DatabaseHandler to add this event in the common queue.
    */
    const std::string& GetSessionId();
    const std::string& GetUuid();
private:
    const std::string& session_id_;
    const std::string& uuid_;
};

/*!
* @brief Added: DatabaseHandler.
*       Handles: NetworkHandler.
*/
class AvatarListIsReady : public Event
{
public:
    using pair = std::pair<int32_t, std::string>;

    AvatarListIsReady(const std::shared_ptr<std::vector<pair>>, const std::string&);
    const std::shared_ptr<std::vector<pair>> GetAvatars();
    //const std::shared_ptr<ProtoMessage::GameMessage> GetEventMessage() const;
        
private:
    //const std::shared_ptr<ProtoMessage::GameMessage> message_;
    
    
    const std::shared_ptr<std::vector<pair>> avatars;
};

/*!
* @brief Added: NetworkHandler.
*       Handles: GameHandler
*/
class StartGameClientRequest : public Event
{
public:
    StartGameClientRequest(const std::string& session_id);
       
private:
   
};

/*!
* @brief Add these events: DatabaseHandler.
*       Handles: GameHandler
*/
class AvatarsDataReady : public Event
{ // !!! Готовы данные конкретного аватара
public:
    AvatarsDataReady(const std::string&, std::shared_ptr<Avatar>);

    std::shared_ptr<Avatar> MoveAvatar();
private:
    const std::shared_ptr<Avatar> avatar_;
};

/*!
* @brief Add these events: NetworkHandler.
*       Handles: DatabaseHandler
*/
class GetAvatarsData : public Event
{
public:
    GetAvatarsData(const std::string& session_id, const uint32_t id_selected_avatar);
    uint32_t GetIdSelectedAvatar();
    const std::string& GetSessionId();
private:
    const uint32_t avatar_id_;
    const std::string& session_id_;
};

/*!
* @brief Added: GameHandler.
*       Handles: NetworkHandler
*/
class StartGameServerResponseSucsses : public Event
{
public:
    StartGameServerResponseSucsses(const std::string& session_id, const std::shared_ptr<Avatar>);

    const std::shared_ptr<const Avatar> GetConstAvatar() const;

private:
    const std::shared_ptr<const Avatar> avatar_;
};

/*!
* @brief Added: NetworkHandler.
*       Handles: DatabaseHandler
*/
class AddItemInInventory : public Event
{
public:
    AddItemInInventory(ItemType, uint32_t avatar_id);
    ItemType GetItem();
    uint32_t GetAvatarId();
private:
    ItemType item_;
    uint32_t avatar_id_;
};

/*!
* @brief Added: sender's NetworkHandler.
*       Handles: all client's NetworkHandler
*/
class BroadcastMessage : public Event
{
public:
    /*!
    * @brief EventDispatcher uses the session_id for filtering
    */
    BroadcastMessage(const std::shared_ptr<ProtoMessage::GameMessage>, 
        const std::string& session_id);

    const std::shared_ptr<ProtoMessage::GameMessage> GetEventMessage() const;
  
private:
    const std::shared_ptr<ProtoMessage::GameMessage> message_;
};

/*!
* @brief Added: NetworkHandler.
*       Handles: GameHandler
*/
class ExitGameRequest : public Event
{
public:
    ExitGameRequest(const std::shared_ptr<ProtoMessage::GameMessage>,  const std::string& session_id);

    const std::shared_ptr<ProtoMessage::GameMessage> GetEventMessage() const;

private:
    const std::shared_ptr<ProtoMessage::GameMessage> message_;
};

/*!
* @brief Added: GameHandler.
*       Handles: DatabaseHandler
*/
class SaveGame : public Event
{
public:
    SaveGame(const std::string& session_id, const std::shared_ptr<Avatar>, bool exit_game = false);

    const std::string& GetSessionId();
    const std::shared_ptr<const Avatar> GetConstAvatar() const;
    bool IsExitGame();
private:
    const std::string& session_id_;
    const std::shared_ptr<const Avatar> avatar_;
    bool exit_game_;
};

/*!
* @brief Added: DatabaseHandler.
*       Handles: NetworkHandler
*/
class ExitGameResponse : public Event
{
public:
    ExitGameResponse(const std::string& session_id);
  

private:
   
};

/*!
* @brief Added: DatabaseHandler.
*       Handles: NetworkHandler
*/
class DisconnectSession : public Event
{
public:
    DisconnectSession(const std::string session_id);


private:
    const std::string session_id_;
};