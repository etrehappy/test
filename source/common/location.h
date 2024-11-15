#pragma once

#include <string>
#include <map>
#include <vector>

//#include "..\source\client\logger.h"
#include "avatar.h"
#include "npc.h"

namespace map_symbols
{
    static const char kWall{'#'};
    static const char kPlayer{'@'};
    static const char kEmpty{' '};
    static const char kDoor{'/'};

    static const char kNpcLora{'L'};
    static const char kNpcAdrik{'A'};

}/* map_symbols */


/*!
*	@brief LocationName = id from database.
*/
enum class LocationName
{

    /* Таверна */
    kTavern = 1

    , kEnd
};

class Location
{
public:

    Location(int id, std::string s_name, std::string description
        , int x, int y, bool is_safe_zone
        , std::map<NpcName, Npc>&& npc_container);

    int GetId() const { return id_; }
    const std::string& GetName() const { return string_name_; }
    const std::string& GetDescription() const { return description_; }
    bool IsSafeZone() const { return is_safe_zone_; }

    void InitPositions(const Avatar& current_avatar);
    void UpdateSymbolsOnMap(const Avatar& current_avatar);
    const std::vector<std::string>& GetMap() const;
    void SetNewPosOnMap(uint32_t x, uint32_t y, const char map_symbol);
    void ClearPosOnMap(uint32_t x, uint32_t y);

private:
    void CreateMap();

    template<LocationName L>
    void SetNpcStandartPos();

    template<LocationName L>
    void AddDoorOnMap();

    int id_;
    std::string string_name_;
    std::string description_;
    int width_, height_;
    bool is_safe_zone_;
    std::map<char, std::pair<uint32_t, uint32_t>> positions_;
    LocationName name_;
    std::map<NpcName, Npc> npc_container_;

    /* location_map[height][width] */
    std::vector<std::string> location_map_;
};
