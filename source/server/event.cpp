#include "event.h"
//

Event::Event(EventPriority p, EventType t, const std::string& key)
    : priority_{p}, type_{t}, event_routing_key_{key}
{
}

EventPriority Event::GetPriority() const
{
    return priority_;
}

EventType Event::GetType() const
{
    return type_;
}

const std::string& Event::GetRoutingKey() const
{
    return event_routing_key_;
}


FirstConnect::FirstConnect(const std::string& session_id)
    : Event{EventPriority::kMedium, EventType::kFirstConnect, session_id}
{
}

LoginRequest::LoginRequest(const std::string& session_id)
    : Event{EventPriority::kMedium, EventType::kUserLogPasServerRequest, session_id}
{
}

LogPasClientResponse::LogPasClientResponse(std::shared_ptr<ProtoMessage::GameMessage> m, const std::string& session_id)
    :Event{EventPriority::kMedium, EventType::kUserLogPasClientResponse, settings::server::kDatabaseEventRoutingKey}, message_{m}, session_id_(session_id)
{
}

const std::shared_ptr<ProtoMessage::GameMessage> LogPasClientResponse::GetEventMessage() const
{
    return message_;
}

const std::string& LogPasClientResponse::GetSessionId()
{
    return session_id_;
}

//AuthenticateAttempt::AuthenticateAttempt(const std::string& id)
//    : Event{EventPriority::kMedium, EventType::kAuthenticateAttempt, id}
//{
//}

SuccessCheckLogPas::SuccessCheckLogPas(const std::string& session_id, std::shared_ptr<const std::string> uuid)
    : Event{EventPriority::kMedium, EventType::kSuccessCheckLogPas, session_id}, uuid_{uuid}
{
}

std::shared_ptr<const std::string> SuccessCheckLogPas::GetUuid()
{
    return uuid_;
}

FailureCheckLogPas::FailureCheckLogPas(const std::string& session_id)
    : Event{EventPriority::kLow, EventType::kFailureCheckLogPas, session_id}
{
}

AvatarListClientRequest::AvatarListClientRequest(const std::string& session_id, const std::string& uuid)
    :Event{EventPriority::kMedium, EventType::kUsersAvatarListClientRequest, settings::server::kDatabaseEventRoutingKey}, session_id_(session_id), uuid_(uuid)
{
}

const std::string& AvatarListClientRequest::GetSessionId()
{
    return session_id_;
}

const std::string& AvatarListClientRequest::GetUuid()
{
    return uuid_;
}

AvatarListIsReady::AvatarListIsReady(const std::shared_ptr<std::vector<pair>> a, const std::string& session_id)
    :Event{EventPriority::kMedium, EventType::kAvatarListIsReady, session_id}, avatars{a}
{
}

const std::shared_ptr<std::vector<std::pair<int32_t, std::string>>> AvatarListIsReady::GetAvatars()
{
    return avatars;
}



StartGameClientRequest::StartGameClientRequest(const std::string& session_id)
    :Event{EventPriority::kMedium, EventType::kStartGameClientRequest, session_id}
{
}

AvatarsDataReady::AvatarsDataReady(const std::string& session_id, std::shared_ptr<Avatar> a)
    : Event{EventPriority::kMedium, EventType::kAvatarsDataReady, session_id}, avatar_{a}
{
}

std::shared_ptr<Avatar> AvatarsDataReady::MoveAvatar()
{    
    return std::move(avatar_);
}

GetAvatarsData::GetAvatarsData(const std::string& session_id, const uint32_t id_selected_avatar)
    : Event{EventPriority::kMedium, EventType::kGetAvatarsData, settings::server::kDatabaseEventRoutingKey}, avatar_id_{id_selected_avatar}, session_id_{session_id}
{
}

uint32_t GetAvatarsData::GetIdSelectedAvatar()
{
    return avatar_id_;
}

const std::string& GetAvatarsData::GetSessionId()
{
    return session_id_;
}

StartGameServerResponseSucsses::StartGameServerResponseSucsses(const std::string& session_id, const std::shared_ptr<Avatar> wp)
    :Event{EventPriority::kMedium, EventType::kStartGameServerResponseSucsses, session_id}, avatar_{wp}
{
    std::cout <<"\n wp: " <<wp;
    std::cout << "\n avatar_: " << avatar_;
}

const std::shared_ptr<const Avatar> StartGameServerResponseSucsses::GetConstAvatar() const
{
    return avatar_;
}

AddItemInInventory::AddItemInInventory(ItemType item, uint32_t avatar_id)
    :Event{EventPriority::kMedium, EventType::kAddItemInInventory, settings::server::kDatabaseEventRoutingKey}, item_{item}, avatar_id_{avatar_id}
{
}

ItemType AddItemInInventory::GetItem()
{
    return item_;
}

uint32_t AddItemInInventory::GetAvatarId()
{
    return avatar_id_;
}

BroadcastMessage::BroadcastMessage(const std::shared_ptr<ProtoMessage::GameMessage> m, const std::string& session_id)
    :Event{EventPriority::kMedium, EventType::kBroadcastMessage, session_id}, message_{m}
{
}

const std::shared_ptr<ProtoMessage::GameMessage> BroadcastMessage::GetEventMessage() const
{
    return message_;
}

ExitGameRequest::ExitGameRequest(const std::shared_ptr<ProtoMessage::GameMessage> m, const std::string& session_id)
    :Event{EventPriority::kEndGame, EventType::kExitGameRequest, session_id}, message_{m}
{
}

const std::shared_ptr<ProtoMessage::GameMessage> ExitGameRequest::GetEventMessage() const
{
    return message_;
}

SaveGame::SaveGame(const std::string& s, const std::shared_ptr<Avatar> a, bool e)
    :Event{EventPriority::kMedium, EventType::kSaveGame, settings::server::kDatabaseEventRoutingKey}
    , session_id_{s}, avatar_{a}, exit_game_{e}
{
}

const std::shared_ptr<const Avatar> SaveGame::GetConstAvatar() const
{
    return avatar_;
}

bool SaveGame::IsExitGame()
{
    return exit_game_;
}

const std::string& SaveGame::GetSessionId()
{
    return session_id_;
}

ExitGameResponse::ExitGameResponse(const std::string& session_id)
    :Event{EventPriority::kMedium, EventType::kExitGameResponse, session_id}
{
}

DisconnectSession::DisconnectSession(const std::string session_id)
    :session_id_{std::move(session_id)}
    , Event {EventPriority::kEndGame, EventType::kDisconnectSession, session_id_}
{
}
