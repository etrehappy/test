#include "location.h"

template<>
void Location::SetNpcStandartPos<LocationName::kTavern>()
{

    const auto& npc_Lora_pos = npc_container_.find(NpcName::kLora)->second.GetPosition();

    positions_[map_symbols::kNpcLora] =
    {npc_Lora_pos.first, npc_Lora_pos.second};

    const auto& npc_Adrik_pos = npc_container_.find(NpcName::kAdrik)->second.GetPosition();



    positions_[map_symbols::kNpcAdrik] =
    {npc_Adrik_pos.first, npc_Adrik_pos.second};

}

template<>
void Location::AddDoorOnMap<LocationName::kTavern>()
{
    location_map_[1][width_ - 2] = map_symbols::kDoor;
}