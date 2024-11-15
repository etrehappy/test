#include "avatar.h"

Avatar::Avatar(uint32_t id, std::string uuid)
    : id_{id}, user_uuid_{std::move(uuid)}
{
}

void Avatar::SetAllNotConstParametrs(std::string n, uint32_t r, uint32_t c, uint32_t lvl, uint32_t e, uint32_t h, uint32_t m, uint32_t s, uint32_t d, uint32_t i, uint32_t loc, int32_t x, int32_t y)
{
    name_ = std::move(n);
    race_id_ = r;
    class_id_ = c;
    level_ = lvl;
    experience_ = e;
    health_ = h;
    mana_ = m;
    strength_ = s;
    dexterity_ = d;
    intelligence_ = i;
    location_id_ = loc;
    position_.first = x;
    position_.second = y;
}

void Avatar::SetFewParametrs(uint32_t lvl, uint32_t e, uint32_t h, uint32_t m, uint32_t s, uint32_t d, uint32_t i, uint32_t loc, int32_t x, int32_t y)
{
    level_ = lvl;
    experience_ = e;
    health_ = h;
    mana_ = m;
    strength_ = s;
    dexterity_ = d;
    intelligence_ = i;
    location_id_ = loc;
    position_.first = x;
    position_.second = y;
}

uint32_t Avatar::GetId() const
{
    return id_;
}

const std::string& Avatar::GetUserUuid() const
{
    return user_uuid_;
}

const std::string& Avatar::GetName() const
{
    return name_;
}

uint32_t Avatar::GetRaceId() const
{
    return race_id_;
}

uint32_t Avatar::GetClassId() const
{
    return class_id_;
}

uint32_t Avatar::GetLevel() const
{
    return level_;
}

uint32_t Avatar::GetExperience() const
{
    return experience_;
}

uint32_t Avatar::GetHealth() const
{
    return health_;
}

uint32_t Avatar::GetMana() const
{
    return mana_;
}

uint32_t Avatar::GetStrength() const
{
    return strength_;
}

uint32_t Avatar::GetDexterity() const
{
    return dexterity_;
}

uint32_t Avatar::GetIntelligence() const
{
    return intelligence_;
}

uint32_t Avatar::GetLocationId() const
{
    return location_id_;
}

std::pair<int32_t, int32_t> Avatar::GetPosition() const
{
    return position_;
}

void Avatar::SetPosX(int32_t x)
{
    position_.first = x;
}

void Avatar::SetPosY(int32_t y)
{
    position_.second = y;
}
