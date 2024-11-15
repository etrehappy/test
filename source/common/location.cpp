#include "location.h"
#include "location.ipp"
#include "..\source\client\logger.h"

Location::Location(int id, std::string s_name, std::string description, int x
	, int y, bool is_safe_zone, std::map<NpcName, Npc>&& npc_container)
	: id_(id), string_name_(std::move(s_name))
	, description_(std::move(description)), width_(x+1), height_(y+1), is_safe_zone_(is_safe_zone)
	, positions_{}, npc_container_{npc_container}, location_map_{}
{
	name_ = static_cast<LocationName>(id_);
}

void Location::InitPositions(const Avatar& current_avatar)
{
    std::pair<int32_t, int32_t> avatar_pos = current_avatar.GetPosition();

    positions_[map_symbols::kPlayer] = {avatar_pos.first, avatar_pos.second};

	switch (name_)
	{
	case LocationName::kTavern:
        CreateMap();
        AddDoorOnMap<LocationName::kTavern>();
		SetNpcStandartPos<LocationName::kTavern>();
		break;
	case LocationName::kEnd:
		break;
	default:
		break;
	}

} /* InitPositions */ 

void Location::UpdateSymbolsOnMap(const Avatar& current_avatar)
{
try
{
    std::pair<int32_t, int32_t> avatar_pos = current_avatar.GetPosition();
    positions_[map_symbols::kPlayer] = {avatar_pos.first, avatar_pos.second};

    for (const auto& [symbol, pos] : positions_)
    {
        location_map_[pos.second][pos.first] = symbol;
    }
}
catch (std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}
} /* UpdateSymbolsOnMap */

const std::vector<std::string>& Location::GetMap() const
{
    return location_map_;
}

void Location::SetNewPosOnMap(uint32_t x, uint32_t y, const char map_symbol)
{
    location_map_[y][x] = map_symbol;
}

void Location::ClearPosOnMap(uint32_t x, uint32_t y)
{
    location_map_[y][x] = map_symbols::kEmpty;
}

void Location::CreateMap()
{
    location_map_ = std::move(
        std::vector<std::string>(height_, std::string(width_, map_symbols::kEmpty))
    );

    /* Create Walls */
    for (int i = 0; i < width_; ++i) 
    {
        location_map_[0][i] = map_symbols::kWall;
        location_map_[height_ - 1][i] = map_symbols::kWall;
    }

    for (int i = 0; i < height_; ++i) 
    {
        location_map_[i][0] = map_symbols::kWall;
        location_map_[i][width_ - 1] = map_symbols::kWall;
    }


} /* CreateMap */