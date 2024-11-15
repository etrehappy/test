#include "db_handler.h"
#include "settings.h"
#include "logger.h"

DatabaseHandler::DatabaseHandler()
    :db_read_{nullptr}
{
    OpenDb();
}

DatabaseHandler::~DatabaseHandler()
{
}

/* thread Listen */
std::shared_ptr<Location> DatabaseHandler::SetLocation(const uint32_t location_id
    , std::mutex& avatar_and_location)
{
try
{
    std::lock_guard g{avatar_and_location};
    
    /*Step 1: location_query*/

    enum cid_loc : uint8_t
    {
        name = 0, description, x, y, is_safe_zone
    };

    SQLite::Statement location_query{*db_read_
        , "SELECT name, description, x, y, is_safe_zone FROM location WHERE id = ?"};
    
    int id_pos = 1;

    location_query.bind(id_pos, location_id);
    
    if (!location_query.executeStep())
    {
        logger::ErrorLocatinReadDb();
        return nullptr;
    }

    /*Step 2: npc_query*/

    std::map<NpcName, Npc> npc_container = std::move(GetNpcListData(location_id));

    if (npc_container.empty())
    {
        logger::ErrorLocatinReadDb();
        return nullptr;
    }

    /*Step 3: Create Location*/
    std::shared_ptr<Location> location = std::make_shared<Location>(location_id
        , std::move(location_query.getColumn(cid_loc::name).getString())
        , std::move(location_query.getColumn(cid_loc::description).getString())
        , location_query.getColumn(cid_loc::x).getInt()
        , location_query.getColumn(cid_loc::y).getInt()
        , location_query.getColumn(cid_loc::is_safe_zone).getInt()
        , std::move(npc_container)
    );

    return location;
}
catch (const std::exception& e)
{
    MVP_LOG_EXEPTION(e)
    return nullptr;
}
}/* SetLocation */

void DatabaseHandler::OpenDb()
{
try
{
    db_read_ = std::make_shared<SQLite::Database>(settings::client::kDatabaseFile
        , SQLite::OPEN_READONLY);

    db_read_->exec("PRAGMA synchronous = NORMAL;");
    db_read_->exec("PRAGMA journal_mode = WAL;");
    db_read_->exec("PRAGMA cache_size = 10000;");
    db_read_->exec("PRAGMA temp_store = MEMORY;");
}
catch (const std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}
}/* OpenDb */


std::map<NpcName, Npc> DatabaseHandler::GetNpcListData(const uint32_t location_id)
{
    std::map<NpcName, Npc> npc_container{};

    enum cid_npc : uint8_t
    {
        id = 0, name, race_id, description, health, pos_x, pos_y
    };

try
{
    SQLite::Statement npc_query{*db_read_
        , "SELECT id, name, race_id, description, health, pos_x, pos_y FROM npc WHERE location_id = ?"};

    int location_id_pos = 1;

    npc_query.bind(location_id_pos, location_id);

    while (npc_query.executeStep())
    {
        const int32_t id_npc = npc_query.getColumn(cid_npc::id).getInt();

        npc_container.emplace(static_cast<NpcName>(id_npc), Npc{
            static_cast<uint32_t>(npc_query.getColumn(cid_npc::id).getInt())
            , npc_query.getColumn(cid_npc::name).getText()
            , static_cast<uint32_t>(npc_query.getColumn(cid_npc::race_id).getInt())
            , npc_query.getColumn(cid_npc::description).getText()
            , static_cast<uint32_t>(npc_query.getColumn(cid_npc::health).getInt())
            , location_id
            , static_cast<uint32_t>(npc_query.getColumn(cid_npc::pos_x).getInt())
            , static_cast<uint32_t>(npc_query.getColumn(cid_npc::pos_y).getInt())}
        );
    }    
}
catch (const std::exception& e)
{
    MVP_LOG_EXEPTION(e)        
}
    return npc_container;

}/* GetNpcListData */