#pragma once

#include <string>

class Avatar
{
public:

    Avatar(uint32_t id, std::string uuid);

    void SetAllNotConstParametrs(std::string name, uint32_t race_id,
        uint32_t class_id, uint32_t level, uint32_t experience, uint32_t health,
        uint32_t mana, uint32_t strength, uint32_t dexterity,
        uint32_t intelligence, uint32_t location_id,
        int32_t x = 0, int32_t y = 0);

    void SetFewParametrs(uint32_t level, uint32_t experience, uint32_t health,
        uint32_t mana, uint32_t strength, uint32_t dexterity,
        uint32_t intelligence, uint32_t location_id,
        int32_t x, int32_t y);

    uint32_t GetId() const;
    const std::string& GetUserUuid() const;
    const std::string& GetName() const;
    uint32_t GetRaceId() const;
    uint32_t GetClassId() const;
    uint32_t GetLevel() const;
    uint32_t GetExperience() const;
    uint32_t GetHealth() const;
    uint32_t GetMana() const;
    uint32_t GetStrength() const;
    uint32_t GetDexterity() const;
    uint32_t GetIntelligence() const;
    uint32_t GetLocationId() const;
    std::pair<int32_t, int32_t> GetPosition() const;
    void SetPosX(int32_t x);
    void SetPosY(int32_t y);

private:
    const uint32_t id_{};
    const std::string user_uuid_{};

    std::string name_{};
    uint32_t race_id_{}, class_id_{}, level_{}, experience_{}, health_{}, mana_{},
        strength_{}, dexterity_{}, intelligence_{}, location_id_{};

    std::pair<int32_t, int32_t> position_{};
};